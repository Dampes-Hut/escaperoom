#include "global.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

#ifndef F3DEX_GBI_PL
#error Does not support non-point-light microcode
#endif

// Toggleable static for debugging, recommended for release is "#define STATIC static"
#ifndef STATIC
#define STATIC
#endif

// Maximum number of lights that can be registered with the system, the 7 (+1 ambient) closest lights to an object's
// position are then selected from these registered lights to be used in drawing that object.
#define LIGHTS_BUFFER_SIZE 32

STATIC struct {
    s32 numOccupied;
    LightNode* free;
    LightNode lights[LIGHTS_BUFFER_SIZE];
} sLightsBuffer;

#define FOR_EACH_LIGHTNODE(cur, head) \
    for (LightNode* (cur) = (head); (cur) != NULL; (cur) = (cur)->next)

//
//  Light Info
//

void Lights_PointSetInfo(LightInfo* info, s16 x, s16 y, s16 z, u8 r, u8 g, u8 b, s16 radius, s32 type) {
    info->type = type;
    info->params.point.x = x;
    info->params.point.y = y;
    info->params.point.z = z;
    Lights_PointSetColorAndRadius(info, r, g, b, radius);
}

void Lights_PointNoGlowSetInfo(LightInfo* info, s16 x, s16 y, s16 z, u8 r, u8 g, u8 b, s16 radius) {
    Lights_PointSetInfo(info, x, y, z, r, g, b, radius, LIGHT_POINT_NOGLOW);
}

void Lights_PointGlowSetInfo(LightInfo* info, s16 x, s16 y, s16 z, u8 r, u8 g, u8 b, s16 radius) {
    Lights_PointSetInfo(info, x, y, z, r, g, b, radius, LIGHT_POINT_GLOW);
}

void Lights_PointSetColorAndRadius(LightInfo* info, u8 r, u8 g, u8 b, s16 radius) {
    info->params.point.color[0] = r;
    info->params.point.color[1] = g;
    info->params.point.color[2] = b;
    info->params.point.radius = radius;
}

void Lights_PointSetPosition(LightInfo* info, s16 x, s16 y, s16 z) {
    info->params.point.x = x;
    info->params.point.y = y;
    info->params.point.z = z;
}

void Lights_DirectionalSetInfo(LightInfo* info, s8 x, s8 y, s8 z, u8 r, u8 g, u8 b) {
    info->type = LIGHT_DIRECTIONAL;
    info->params.dir.x = x;
    info->params.dir.y = y;
    info->params.dir.z = z;
    info->params.dir.color[0] = r;
    info->params.dir.color[1] = g;
    info->params.dir.color[2] = b;
}

//
//  Light Context
//

void LightContext_Init(PlayState* play, LightContext* lightCtx) {
    lightCtx->listHead = NULL;
    lightCtx->ambientColor[0] = lightCtx->ambientColor[1] = lightCtx->ambientColor[2] = 80;
    lightCtx->fogColor[0] = lightCtx->fogColor[1] = lightCtx->fogColor[2] = 0;
    lightCtx->fogNear = 996;
    lightCtx->zFar = 12800;

    sLightsBuffer.numOccupied = 0;
    sLightsBuffer.free = &sLightsBuffer.lights[0];
    bzero(&sLightsBuffer.lights, sizeof(sLightsBuffer.lights));
}

void LightContext_DestroyList(PlayState* play, LightContext* lightCtx) {
    FOR_EACH_LIGHTNODE(lightNode, lightCtx->listHead) {
        LightContext_RemoveLight(play, lightCtx, lightNode);
    }
}

//
//  Light List
//

/**
 * Finds an available slot in the lights buffer. It's unclear why it was originally designed like this considering the
 * LightNode could've just been in for example an actor struct rather than in a dedicated buffer for all of them, but
 * it's too large a change currently to completely redo this.
 */
STATIC LightNode* Lights_FindBufSlot(void) {
    if (sLightsBuffer.numOccupied >= LIGHTS_BUFFER_SIZE) {
        // No free slots
        return NULL;
    }

    // Guaranteed to be free (tm) if the buffer is not full
    LightNode* light = sLightsBuffer.free;
    assert(light->info == NULL);

    sLightsBuffer.numOccupied++;

    // If we search for a free slot in a full buffer we'd get an infinite loop, the downside is this leaves the free
    // pointer dangling until a slot becomes free. Should be fine since we check if the buffer is full earlier..
    if (sLightsBuffer.numOccupied < LIGHTS_BUFFER_SIZE) {
        // Search the buffer for the next free slot (that is not the slot we are about to return)
        // The wrapping behavior may be concerning, as we might wrap back to the slot we are about to return, however
        // there exists at least one other empty slot that will be found before then, as the buffer is not full.
        do {
            sLightsBuffer.free++;
            if (sLightsBuffer.free >= &sLightsBuffer.lights[LIGHTS_BUFFER_SIZE]) {
                // Overran the buffer, return to the start of the buffer and keep searching
                sLightsBuffer.free = &sLightsBuffer.lights[0];
            }
            // Continue until we find the next empty slot
        } while (sLightsBuffer.free->info != NULL);

        assert(sLightsBuffer.free->info == NULL);
    }
    return light;
}

/**
 * Insert a new light into the current light list.
 *
 * Note: Due to the limited number of slots in a Lights group, inserting too many lights in the list will result in
 * only the closest lights to the object or camera eye to be bound when drawing. Further, inserting too many lights
 * into the system as a whole will eventually cause lights to be silently dropped.
 */
LightNode* LightContext_InsertLight(PlayState* play, LightContext* lightCtx, LightInfo* info) {
    LightNode* light = Lights_FindBufSlot();
    if (light == NULL) {
        // No space left
        return NULL;
    }

    // Bind light info
    light->info = info;

    // Link
    light->prev = NULL;
    light->next = lightCtx->listHead;
    if (lightCtx->listHead != NULL) {
        lightCtx->listHead->prev = light;
    }
    lightCtx->listHead = light;

    return light;
}

/**
 * Removes a light from the current light list.
 */
void LightContext_RemoveLight(PlayState* play, LightContext* lightCtx, LightNode* light) {
    if (light == NULL) {
        // The light ended up unallocated due to lack of space, silently return
        return;
    }

    // Unlink
    if (light->prev != NULL) {
        light->prev->next = light->next;
    } else {
        lightCtx->listHead = light->next;
    }
    if (light->next != NULL) {
        light->next->prev = light->prev;
    }

    // Unbind light info
    light->info = NULL;

    // Update lights buffer
    sLightsBuffer.numOccupied--;
    // Move the free pointer to the most recently removed light, this kind-of wastes some of the effort we go through
    // when inserting a light to set the free pointer however it is expected that lights are added in bulk and removed
    // in bulk rather than additions and removals being interleaved repeatedly.
    sLightsBuffer.free = light;
}

/**
 * Insert a list of lights into the current light list. The same length limitations apply as with inserting single
 * lights, see `LightContext_InsertLight` for details.
 *
 * @param lightList The list of light structures to insert.
 * @param numLights Pointer to the number of light structures to insert. The value held may be shrunk to the actual
 *                  number of lights inserted if some lights were dropped due to running out of space.
 *
 * @return Pointer to the first LightNode structure for this list. Can be passed to `LightContext_RemoveLightList`
 *         along with the output in `numLights` to deallocate the entire list.
 *
 * @see LightContext_InsertLight
 * @see LightContext_RemoveLightList
 */
LightNode* LightContext_InsertLightList(PlayState* play, LightContext* lightCtx, LightInfo* lightList, u32* numLights) {
    LightNode* first = NULL;
    u32 numLightsIn = *numLights;
    u32 i;

    for (i = 0; i < numLightsIn; i++) {
        // Try and insert the light
        LightNode* light = LightContext_InsertLight(play, lightCtx, lightList++);

        if (light == NULL) {
            // No room, return the true number of lights inserted for possible removal later
            *numLights = i;
            break;
        }
        if (first == NULL) {
            // Record pointer to first light for possible removal later
            first = light;
        }
    }
    return first;
}

/**
 * Removes `numLights` starting at `firstLight` from the current light list.
 */
void LightContext_RemoveLightList(PlayState* play, LightContext* lightCtx, LightNode* firstLight, u32 numLights) {
    LightNode* light = firstLight;
    u32 i;

    for (i = 0; i < numLights && light != NULL; i++) {
        LightNode* next = light->next;
        LightContext_RemoveLight(play, lightCtx, light);
        light = next;
    }
}

//
//  Light Binding
//

STATIC Light* Lights_FindSlot(Lights* lights, f32 objDist) {
    if (lights->numLights < MAX_LIGHTS) {
        // There's still a free slot, add it unconditionally.
        lights->distances[lights->numLights] = objDist;
        return &lights->l.l[lights->numLights++];
    }

    // Search for the slot with the furthest light from the object being lit
    f32 furthestDist = objDist;
    s32 furthestI = -1;
    for (s32 i = 0; i < MAX_LIGHTS; i++) {
        if (furthestDist < lights->distances[i]) {
            // Largest distance found in the list so far, note it down
            furthestDist = lights->distances[i];
            furthestI = i;
        }
    }
    if (furthestI == -1) {
        // The light we're trying to add is the furthest light, reject it.
        return NULL;
    }

    // Replace the furthest light with the new light
    lights->distances[furthestI] = furthestDist;
    return &lights->l.l[furthestI];
}

STATIC void Lights_BindPointWithReference(Lights* lights, LightParams* params, Vec3f* objPos, PlayState* play) {
    if (params->point.radius <= 0) {
        return;
    }

    Vec3f posF;
    posF.x = params->point.x;
    posF.y = params->point.y;
    posF.z = params->point.z;

    Vec3f refDiff;
    refDiff.x = posF.x - objPos->x;
    refDiff.y = posF.y - objPos->y;
    refDiff.z = posF.z - objPos->z;

    f32 refDistSq = SQ(refDiff.x) + SQ(refDiff.y) + SQ(refDiff.z);
    if (SQ(params->point.radius) <= refDistSq) {
        return;
    }

    f32 refDist = sqrtf(refDistSq);

    // Try to add this light to the group

    Light* light = Lights_FindSlot(lights, refDist);
    if (light == NULL) {
        // Fail, quit early.
        return;
    }

    // Success, build the light structure

    f32 scale = refDist / params->point.radius;

    scale = 1 - SQ(scale);

    light->l.col[0] = light->l.colc[0] = params->point.color[0] * scale;
    light->l.col[1] = light->l.colc[1] = params->point.color[1] * scale;
    light->l.col[2] = light->l.colc[2] = params->point.color[2] * scale;

    scale = (refDist < 1) ? 120 : 120 / refDist;

    light->l.dir[0] = refDiff.x * scale;
    light->l.dir[1] = refDiff.y * scale;
    light->l.dir[2] = refDiff.z * scale;

    light->l.pad1 = 0; // not a point light
}

STATIC void Lights_BindPoint(Lights* lights, LightParams* params, Vec3f* objPos, PlayState* play) {
    f32 radiusF = params->point.radius;
    if (radiusF <= 0) {
        return;
    }

    Vec3f posF;
    posF.x = params->point.x;
    posF.y = params->point.y;
    posF.z = params->point.z;

    // TODO is this really needed? MM does it but it probably doesn't noticeably improve performance
    // and just makes lights abruptly appear/disappear. Maybe it's to avoid fixed-point overflows in
    // the microcode? Who knows..
#if 0
    // On-screen check

    Vec3f screenPos;
    SkinMatrix_Vec3fMtxFMultXYZ(&play->viewProjectionMtxF, &posF, &screenPos);

    if ((screenPos.z <= -radiusF) || (600 <= screenPos.z - radiusF) || (400 <= fabsf(screenPos.x) - radiusF) ||
        (400 <= fabsf(screenPos.y) - radiusF)) {
        return;
    }
#endif

    // Try to add this light to the group

    Light* light = Lights_FindSlot(lights, Math_Vec3f_DistXYZ(objPos, &posF));
    if (light == NULL) {
        // Fail, quit early.
        return;
    }

    // Success, build the light structure

    radiusF = 4500000.0f / SQ(radiusF);
    if (radiusF > 255) {
        radiusF = 255;
    } else if (radiusF < 20) {
        radiusF = 20;
    }

    light->p.col[0] = light->p.colc[0] = params->point.color[0];
    light->p.col[1] = light->p.colc[1] = params->point.color[1];
    light->p.col[2] = light->p.colc[2] = params->point.color[2];

    light->p.pos[0] = params->point.x;
    light->p.pos[1] = params->point.y;
    light->p.pos[2] = params->point.z;

    // Set attenuation values, light intensity is computed as L = L0 / (qa * SQ(d) + la * d + ca) in the microcode
    // where d is the distance between a vertex and the light position. `ca` must be non-zero for the microcode to
    // take the point lighting codepath.
    light->p.ca = 8;
    light->p.la = 255;
    light->p.qa = (s32)radiusF;
}

STATIC void Lights_BindDirectional(Lights* lights, LightParams* params, UNUSED Vec3f* objPos, UNUSED PlayState* play) {
    // objPos is ignored for directional lights

    // Treat directional lights as being as close as possible to the camera eye so they are always considered
    // (except in the case where there are too many directional lights)
    Light* light = Lights_FindSlot(lights, 0.0f);
    if (light == NULL) {
        return;
    }

    light->l.col[0] = light->l.colc[0] = params->dir.color[0];
    light->l.col[1] = light->l.colc[1] = params->dir.color[1];
    light->l.col[2] = light->l.colc[2] = params->dir.color[2];

    light->l.dir[0] = params->dir.x;
    light->l.dir[1] = params->dir.y;
    light->l.dir[2] = params->dir.z;

    light->l.pad1 = 0; // not a point light
}

STATIC void Lights_BindDummy(UNUSED Lights* lights, UNUSED LightParams* params, UNUSED Vec3f* objPos,
                             UNUSED PlayState* play) {
    // Empty
}

//
//  Light Drawing
//

/**
 * For every light registered with the Light Context, build a Lights group containing the 7 lights that are of the
 * highest priority. Directional lights are given highest priority, point lights (real and "fake") are given priority
 * based on proximity to `objPos`, or the camera eye position if `objPos` is NULL, as these lights will have the
 * greatest visible effect on the geometry they are intended to light.
 * This system will fail to alleviate visual artifacts caused by the 7 light limit if too many lights are crammed into
 * a small area.
 *
 * Once the Lights group has been constructed, lighting gbi commands will be appended to the POLY_OPA_DISP and
 * POLY_XLU_DISP buffers so that subsequently drawn geometry will be affected by these lights.
 *
 * @param play
 *      Play Game State.
 * @param objPos
 *      The position of the object these lights are for. Used for computing the direction for "fake" point lights and
 *      for selecting which lights to prioritize when there are more than 7 lights registered. May be NULL, in which
 *      case the current active camera's eye position is used instead to judge which lights should be prioritized.
 * @param realPointLights
 *      Boolean indicating whether the geometry that will be drawn with these lights supports the use of real point
 *      lighting.
 *      Geometry allows real point lights if whenever G_LIGHTING is on, G_LIGHTING_POSITIONAL is also on. If G_LIGHTING
 *      were on and G_LIGHTING_POSITIONAL were off and we used real point light structures, the microcode would
 *      interpret the PointLight_t structure as a Light_t structure leading to problems e.g. flickering geometry.
 *      Ensure that whenever this is true the geometry that is subsequently drawn is compatible.
 *
 * @return The resulting Lights group containing all lights that have been selected by highest priority.
 *         NOTE this data is only valid until the end of the current frame, do not store pointers to this data in any
 *         persistent location!
 */
Lights* Lights_BindAndDraw(PlayState* play, Vec3f* objPos, s32 realPointLights) {
    // Real point light bind functions
    static const LightsBindFunc posBindFuncs[] = {
        Lights_BindPoint,
        Lights_BindDirectional,
        Lights_BindPoint,
    };
    // "Fake" point light bind functions
    static const LightsBindFunc dirBindFuncs[] = {
        Lights_BindPointWithReference,
        Lights_BindDirectional,
        Lights_BindPointWithReference,
    };
    // No point light bind function
    static const LightsBindFunc dirOnlyBindFuncs[] = {
        Lights_BindDummy,
        Lights_BindDirectional,
        Lights_BindDummy,
    };

    // Select bind functions
    const LightsBindFunc* bindFuncs;
    if (realPointLights) {
        // If the geometry that will be drawn with these lights allow real point lights, always use them.
        // Geometry allows real point lights if whenever G_LIGHTING is on, G_LIGHTING_POSITIONAL is also on. If
        // G_LIGHTING were on and G_LIGHTING_POSITIONAL were off and we used real point light structures, the microcode
        // would interpret the PointLight_t structure as a Light_t structure leading to problems e.g. flickering.
        bindFuncs = posBindFuncs;
    } else if (objPos == NULL) {
        // If there is no defined object position, e.g. for lighting room geometry or a group of objects, we can't use
        // the "fake" point lights either so we select the dummy bind functions that totally ignore point-type lights.
        // This mirrors vanilla oot behavior.
        bindFuncs = dirOnlyBindFuncs;
    } else {
        // We don't allow real point lights, likely because the material settings of whatever we're going to draw does
        // not properly support them, but we have a defined object position. In this case allow "fake" point lights.
        bindFuncs = dirBindFuncs;
    }

    // If there was no defined object position, select camera eye position as the reference position for prioritizing
    // lights. The hope is this gives an approximately correct image overall.
    if (objPos == NULL) {
        Camera* activeCam = GET_ACTIVE_CAM(play);
        // TODO lighting: can this ever be NULL?
        assert(activeCam != NULL);
        objPos = &activeCam->eye;
    }

    // Allocate lights group, deallocates by next frame
    Lights* lights = Graph_Alloc(play->state.gfxCtx, sizeof(Lights));

    // Configure ambient color
    lights->l.a.l.col[0] = lights->l.a.l.colc[0] = play->lightCtx.ambientColor[0];
    lights->l.a.l.col[1] = lights->l.a.l.colc[1] = play->lightCtx.ambientColor[1];
    lights->l.a.l.col[2] = lights->l.a.l.colc[2] = play->lightCtx.ambientColor[2];

    // Set number of lights in the group to 0
    lights->numLights = 0;
    // Note that we do not initialize the lights->distances array here, as by the time anything is read from that array
    // all of the array elements will have been set

    // Populate lights group
    FOR_EACH_LIGHTNODE(lightNode, play->lightCtx.listHead) {
        LightInfo* info = lightNode->info;
        bindFuncs[info->type](lights, &info->params, objPos, play);
    }

    // Apply the lights to the gfx command list
    OPEN_DISPS(play->state.gfxCtx);

    assert(lights->numLights <= MAX_LIGHTS);

    // Set number of lights
    gSPNumLights(POLY_OPA_DISP++, lights->numLights);
    gSPNumLights(POLY_XLU_DISP++, lights->numLights);

    // Directional and Point lights
    Light* light;
    s32 i;
    for (light = &lights->l.l[0], i = 0; i < lights->numLights; i++, light++) {
        gSPLight(POLY_OPA_DISP++, light, 1 + i);
        gSPLight(POLY_XLU_DISP++, light, 1 + i);
    }

    // Ambient Light
    gSPLight(POLY_OPA_DISP++, &lights->l.a, 1 + i);
    gSPLight(POLY_XLU_DISP++, &lights->l.a, 1 + i);

    CLOSE_DISPS(play->state.gfxCtx);

    return lights;
}

void Lights_GlowCheck(PlayState* play) {
    FOR_EACH_LIGHTNODE(lightNode, play->lightCtx.listHead) {
        if (lightNode->info->type != LIGHT_POINT_GLOW) {
            continue;
        }

        LightPoint* params = &lightNode->info->params.point;
        Vec3f pos;
        Vec3f multDest;
        f32 cappedInvWDest;
        f32 wX;
        f32 wY;

        pos.x = params->x;
        pos.y = params->y;
        pos.z = params->z;
        Actor_ProjectPos(play, &pos, &multDest, &cappedInvWDest);
        params->drawGlow = false;
        wX = multDest.x * cappedInvWDest;
        wY = multDest.y * cappedInvWDest;

        if ((multDest.z > 1.0f) && (fabsf(wX) < 1.0f) && (fabsf(wY) < 1.0f)) {
            s32 wZ = (s32)((multDest.z * cappedInvWDest) * ((G_MAXZ / 2) * 32)) + (G_MAXZ / 2) * 32;
            s32 zVal = gZBuffer[(s32)((wY * -(SCREEN_HEIGHT / 2)) + (SCREEN_HEIGHT / 2))]
                               [(s32)((wX * (SCREEN_WIDTH / 2)) + (SCREEN_WIDTH / 2))];

            if (wZ < (Environment_ZBufValToFixedPoint(zVal << 2) >> 3)) {
                params->drawGlow = true;
            }
        }
    }
}

STATIC Gfx sGlowSetupDL[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0, 0, 0, 0, PRIMITIVE, TEXEL0, 0, PRIMITIVE, 0),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                     G_AC_THRESHOLD | G_ZS_PIXEL | G_RM_CLD_SURF | G_RM_CLD_SURF2),
    gsSPLoadGeometryMode(G_SHADE | G_SHADING_SMOOTH),
    gsSPBranchList(gGlowCircleTextureLoadDL),
};

void Lights_DrawGlow(PlayState* play) {
    if (play->lightCtx.listHead == NULL) {
        // Early return to avoid appending sGlowSetupDL if there are no lights at all.
        return;
    }

    s32 hasGlow = false;
    FOR_EACH_LIGHTNODE(lightNode, play->lightCtx.listHead) {
        if (lightNode->info->type == LIGHT_POINT_GLOW) {
            hasGlow = true;
            break;
        }
    }
    if (!hasGlow) {
        // Early return to avoid appending sGlowSetupDL if there are no POINT_GLOW lights.
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    gSPDisplayList(POLY_XLU_DISP++, sGlowSetupDL);

    FOR_EACH_LIGHTNODE(lightNode, play->lightCtx.listHead) {
        if (lightNode->info->type != LIGHT_POINT_GLOW) {
            continue;
        }

        LightPoint* params = &lightNode->info->params.point;
        if (!params->drawGlow) {
            continue;
        }

        f32 scale = SQ(params->radius) * 2e-6f;
        Matrix_Translate(params->x, params->y, params->z, MTXMODE_NEW);
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx, __FILE__, __LINE__),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, params->color[0], params->color[1], params->color[2], 50);
        gSPDisplayList(POLY_XLU_DISP++, gGlowCircleDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
