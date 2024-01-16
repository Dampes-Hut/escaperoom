#include "global.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

#undef LightContext_InsertLight
#undef LightContext_InsertLightList

#ifndef F3DEX_GBI_PL
#error Does not support non-point-light microcode
#endif

// Toggleable static for debugging, recommended for release is "#define STATIC static"
#ifndef STATIC
#define STATIC
#endif

// Maximum number of lights that can be registered with the system, the 7 (+1 ambient) closest lights to an object's
// position are then selected from these registered lights to be used in drawing that object.
// Note this is fixed to 32 unless the occupiedBitSet also changes size.
#define LIGHTS_BUFFER_SIZE 32

typedef struct {
    u32 occupiedBitSet;
    LightNode lights[LIGHTS_BUFFER_SIZE];
} LightsBuffer;
STATIC LightsBuffer sLightsBuffer;

#define FOR_EACH_LIGHTNODE(cur, head) for (LightNode * (cur) = (head); (cur) != NULL; (cur) = (cur)->next)

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

    sLightsBuffer.occupiedBitSet = 0;
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
    static_assert(LIGHTS_BUFFER_SIZE == 32, "Bitset handling assumes the light buffer is 32 entries large.");

    u32 bitset = sLightsBuffer.occupiedBitSet;

    if (bitset == 0xFFFFFFFF) {
        // No free slots
        return NULL;
    }

    // Find the first zero
    u32 i = 0;
    while (bitset & 1) {
        bitset >>= 1;
        i++;
    }

    // Get the light, should be free
    LightNode* light = &sLightsBuffer.lights[i];
    assert(light->info == NULL);

    // Mark it as occupied
    sLightsBuffer.occupiedBitSet |= 1 << i;
    return light;
}

/**
 * Insert a new light into the current light list.
 *
 * Note: Due to the limited number of slots in a Lights group, inserting too many lights in the list will result in
 * only the closest lights to the object or camera eye to be bound when drawing. Further, inserting too many lights
 * into the system as a whole will eventually cause lights to be silently dropped.
 */
LightNode* LightContext_InsertLight(PlayState* play, LightContext* lightCtx, LightInfo* info
#ifndef NDEBUG
                                    ,
                                    char* file, int line
#endif
) {
#ifndef NDEBUG
    // Check not inserting the light a second time
    FOR_EACH_LIGHTNODE(lightNode, lightCtx->listHead) {
        assert(lightNode->info != info);
    }
#endif

    LightNode* light = Lights_FindBufSlot();
    if (light == NULL) {
        // No space left
        return NULL;
    }

#ifndef NDEBUG
    assert(file != NULL);
    size_t len = strlen(file);
    if (len >= sizeof(light->file))
        len = sizeof(light->file) - 1;
    assert(len < sizeof(light->file));
    memcpy(light->file, file, len);
    light->file[len] = '\0';
    light->line = line;
    light->additional_context[0] = '\0';
#endif

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

    // The light pointer must be in the lights buffer
    assert(light >= &sLightsBuffer.lights[0] && light < &sLightsBuffer.lights[LIGHTS_BUFFER_SIZE]);
    // Index in the light buffer
    u32 i = light - &sLightsBuffer.lights[0];

    // Check for double free
    assert(light->info != NULL && (sLightsBuffer.occupiedBitSet & (1 << i)));

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
#ifndef NDEBUG
    light->file[0] = '\0';
    light->line = -1;
    const char additional_context_freed[] = "freed by LightContext_RemoveLight";
    memcpy(light->additional_context, additional_context_freed, sizeof(additional_context_freed));
#endif
    // Mark not occupied
    sLightsBuffer.occupiedBitSet &= ~(1 << i);
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
LightNode* LightContext_InsertLightList(PlayState* play, LightContext* lightCtx, LightInfo* const lightList,
                                        u32* numLights
#ifndef NDEBUG
                                        ,
                                        char* file, int line
#endif
) {
    LightNode* first = NULL;
    u32 numLightsIn = *numLights;
    u32 i;

#ifndef NDEBUG
    // Check not inserting any light a second time
    FOR_EACH_LIGHTNODE(lightNode, lightCtx->listHead) {
        for (i = 0; i < numLightsIn; i++) {
            assert(lightNode->info != &lightList[i]);
        }
    }
#endif

    for (i = 0; i < numLightsIn; i++) {
        // Try and insert the light
        LightNode* light = LightContext_InsertLight(play, lightCtx, &lightList[i]
#ifndef NDEBUG
                                                    ,
                                                    file, line
#endif
        );

        if (light == NULL) {
            // No room, return the true number of lights inserted for possible removal later
            *numLights = i;
            break;
        }

        // LightContext_InsertLight inserts lights by adding them at the start
        // of the lightCtx->listHead linked list
        // This means the first light corresponding to the added lights in
        // order of forward traversal of lightCtx->listHead, is the last
        // inserted light.
        // Record pointer to first light for possible removal later
        first = light;
    }

#ifndef NDEBUG
    // Check the lights were inserted in the expected order
    // (and are found in reverse order compared to the input array)
    // Otherwise the first node `first` would most likely be wrong
    i = *numLights;
    FOR_EACH_LIGHTNODE(lightNode, first) {
        if (i == 0) {
            break;
        }
        i--;
        assert(lightNode->info == &lightList[i]);
    }
    // Check all the inserted lights were indeed inserted
    assert(i == 0);
#endif

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

STATIC void Lights_BindPointWithReference(Lights* restrict lights, LightParams* restrict params, Vec3f* objPos,
                                          UNUSED PlayState* play) {
    if (params->point.radius <= 0) {
        return;
    }
    // Ignore (0,0,0)-colored lights
    if ((params->point.color[0] | params->point.color[1] | params->point.color[2]) == 0) {
        return;
    }
    f32 radiusF = params->point.radius;

    Vec3f posF;
    posF.x = params->point.x;
    posF.y = params->point.y;
    posF.z = params->point.z;

    Vec3f refDiff;
    refDiff.x = posF.x - objPos->x;
    refDiff.y = posF.y - objPos->y;
    refDiff.z = posF.z - objPos->z;

    f32 refDistSq = SQ(refDiff.x) + SQ(refDiff.y) + SQ(refDiff.z);
    if (SQ(radiusF) <= refDistSq) {
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

    f32 scale = refDist / radiusF;

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

STATIC void Lights_BindPoint(Lights* restrict lights, LightParams* restrict params, Vec3f* objPos, PlayState* play) {
    if (params->point.radius <= 0) {
        return;
    }
    // Ignore (0,0,0)-colored lights
    if ((params->point.color[0] | params->point.color[1] | params->point.color[2]) == 0) {
        return;
    }
    f32 radiusF = params->point.radius;

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

STATIC void Lights_BindDirectional(Lights* restrict lights, LightParams* restrict params, UNUSED Vec3f* objPos,
                                   UNUSED PlayState* play) {
    // objPos is ignored for directional lights

    // Ignore (0,0,0)-colored lights
    if ((params->dir.color[0] | params->dir.color[1] | params->dir.color[2]) == 0) {
        return;
    }

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

typedef struct LightsBindAndDrawFaultClientInfo {
    PlayState* play;
    Vec3f* objPos;
    s32 realPointLights;
} LightsBindAndDrawFaultClientInfo;

#define FaultDrawer_and_rmon_Printf(...) \
    {                                    \
        rmonPrintf(__VA_ARGS__);         \
        FaultDrawer_Printf(__VA_ARGS__); \
    }                                    \
    (void)0

s32 Lights_BindAndDraw_FaultClient(void* arg0, void* arg1) {
    LightsBindAndDrawFaultClientInfo* i = arg0;
    if (i == NULL) {
        FaultDrawer_and_rmon_Printf("i == NULL\n");
        return false;
    }
    FaultDrawer_and_rmon_Printf("objPos = 0x%08X\n", (u32)i->objPos);
    if (i->objPos != NULL)
        FaultDrawer_and_rmon_Printf("*objPos = %f %f %f\n", XYZ(*i->objPos));
    FaultDrawer_and_rmon_Printf("realPointLights = %d\n", (int)i->realPointLights);
    if (i->play == NULL) {
        FaultDrawer_and_rmon_Printf("i->play == NULL\n");
        return false;
    }
    FaultDrawer_and_rmon_Printf("i->play->lightCtx.listHead = 0x%08X\n", (u32)i->play->lightCtx.listHead);
    for (LightNode* ln = i->play->lightCtx.listHead; ln != NULL; ln = ln->next) {
        FaultDrawer_and_rmon_Printf("ln = 0x%08X\n", (u32)ln);
        FaultDrawer_and_rmon_Printf("ln->info = 0x%08X\n", (u32)ln->info);
        if ((u32)ln->info >= PHYS_TO_K0(0) && ((u32)ln->info + sizeof(*ln->info)) < PHYS_TO_K0(osMemSize)) {
            FaultDrawer_and_rmon_Printf("ln->info->type = %d\n", (int)ln->info->type);
        } else {
            FaultDrawer_and_rmon_Printf("ln->info out of K0 ram, most likely a bad pointer\n");
        }
        FaultDrawer_and_rmon_Printf("ln->file = %.*s\n", sizeof(ln->file), ln->file);
        FaultDrawer_and_rmon_Printf("ln->line = %d\n", ln->line);
        FaultDrawer_and_rmon_Printf("ln->additional_context = %.*s\n", sizeof(ln->additional_context),
                                    ln->additional_context);
    }
    return false;
}

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
#ifndef NDEBUG
    FaultClient __attribute__((__cleanup__(Fault_RemoveClient))) faultClient;
    LightsBindAndDrawFaultClientInfo lightsBindAndDrawFaultClientInfo = {
        .play = play,
        .objPos = objPos,
        .realPointLights = realPointLights,
    };

    Fault_AddClient(&faultClient, Lights_BindAndDraw_FaultClient, &lightsBindAndDrawFaultClientInfo, NULL);
#endif

    // Real point light bind functions
    static const LightsBindFunc posBindFuncs[LIGHT_TYPE_MAX] = {
        Lights_BindPoint,
        Lights_BindDirectional,
        Lights_BindPoint,
    };
    // "Fake" point light bind functions
    static const LightsBindFunc dirBindFuncs[LIGHT_TYPE_MAX] = {
        Lights_BindPointWithReference,
        Lights_BindDirectional,
        Lights_BindPointWithReference,
    };
    // No point light bind function
    static const LightsBindFunc dirOnlyBindFuncs[LIGHT_TYPE_MAX] = {
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
        assert(info->type >= 0);
        assert(info->type < LIGHT_TYPE_MAX);
        assert((uintptr_t)(bindFuncs[info->type]) >= PHYS_TO_K0(0));
        assert((uintptr_t)(bindFuncs[info->type]) < PHYS_TO_K0(osMemSize));
        bindFuncs[info->type](lights, &info->params, objPos, play);
    }

    // Better not go over the ucode limitation
    assert(lights->numLights <= MAX_LIGHTS);

    if (lights->numLights == 0) {
        // Another ucode limitation: Lights0 still requires at least one other light alongside ambient.. add a dummy
        lights->l.l[0] = (Light){ 0 };
        lights->numLights = 1;
    }

    // Apply the lights to the gfx command list
    OPEN_DISPS(play->state.gfxCtx);

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

STATIC void Lights_PopOne(Gfx* dl) {
#define GBI_IS_NOOP(gW0) (((gW0) >> 24) == G_NOOP)
#define GBI_IS_LIGHTS(gW0) (((gW0) & 0xFFFF00FF) == (((Gfx)gsSPLight(0, 0)).words.w0 & 0xFFFF00FF))
#define GBI_IS_NUMLIGHTS(gW0) ((gW0) == ((Gfx)gsSPNumLights(0)).words.w0)

    dl--;
    u32 gW0 = dl->words.w0;

    if (GBI_IS_NOOP(gW0)) {
        // gsDPNoOpCloseDisp
        // ignore
        dl--;
        gW0 = dl->words.w0;
    }

    while (GBI_IS_LIGHTS(gW0)) {
        // gsSPLight
        // replace with no-op
        gDPNoOpHere(dl, __FILE__, __LINE__);

        dl--;
        gW0 = dl->words.w0;
    }

    if (GBI_IS_NUMLIGHTS(gW0)) {
        // gsSPNumLights
        // replace with no-op
        gDPNoOpHere(dl, __FILE__, __LINE__);
    }
}

/**
 * Deletes lights assignments that were not used from the previous Lights_BindAndDraw call
 */
void Lights_Pop(PlayState* play) {
    Lights_PopOne(play->state.gfxCtx->polyOpa.p);
    Lights_PopOne(play->state.gfxCtx->polyXlu.p);
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
