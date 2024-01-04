#include "inn_painting.h"

#include "assets/objects/object_poh/object_poh.h"
#include "assets_mod/objects/object_inn_painting/gObjectInnPaintingBackgroundDL.h"

#define FLAGS 0

void InnPainting_Init(Actor* thisx, PlayState* play);
void InnPainting_Destroy(Actor* thisx, PlayState* play);
void InnPainting_WaitObjects(Actor* thisx, PlayState* play);
void InnPainting_Update(Actor* thisx, PlayState* play);
void InnPainting_Draw(Actor* thisx, PlayState* play);

void InnPainting_MallocBuffers(void);
void InnPainting_FreeBuffers(void);

ActorInit InnPainting_InitVars = {
    /**/ ACTOR_INN_PAINTING,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(InnPaintingActor),
    /**/ InnPainting_Init,
    /**/ InnPainting_Destroy,
    /**/ InnPainting_WaitObjects,
    /**/ NULL,
};

void InnPainting_Init(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    this->objectPohSlot = Object_GetSlot(&play->objectCtx, OBJECT_POH);
    assert(this->objectPohSlot >= 0);
    this->objectInnPaintingSlot = Object_GetSlot(&play->objectCtx, OBJECT_INN_PAINTING);
    assert(this->objectInnPaintingSlot >= 0);

    Actor_SetScale(thisx, 0.1f);
    InnPainting_MallocBuffers();
}

void InnPainting_Destroy(Actor* thisx, PlayState* play) {
    InnPainting_FreeBuffers();
}

void InnPainting_WaitObjects(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    if (Object_IsLoaded(&play->objectCtx, this->objectPohSlot) &&
        Object_IsLoaded(&play->objectCtx, this->objectInnPaintingSlot)) {

        gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.slots[this->objectPohSlot].segment);
        SkelAnime_Init(play, &this->poeSkelAnime, &gPoeSkel, &gPoeFloatAnim, NULL, NULL, 0);
        this->poeSkelAnime.playSpeed = 0.0f;
        this->poeSkelAnime.curFrame = 5.0f;

        this->actor.update = InnPainting_Update;
        this->actor.draw = InnPainting_Draw;
    }
}

void InnPainting_Update(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    this->timer++;

    gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.slots[this->objectPohSlot].segment);
    SkelAnime_Update(&this->poeSkelAnime);
}

/**
 *       ^s
 *      /
 *     *2
 *    /|
 *   / |  y^
 *  /  |   | ^x
 * *3  |   |/
 * |   *1  +-->z
 * |  /
 * | /
 * |/
 * *0
 * |
 * vt
 *
 * 3: top-left, s,t=0,0
 * 1: bottom-right, s,t=texture corner opposite of 0,0
 * 0: x,y=0,0
 *
 * Meant to be lit (G_LIGHTING), all normals are +z
 */
#define IPQUAD_W 80
#define IPQUAD_H 20
#define IPQUAD_TEXW 64
#define IPQUAD_TEXH 16
static Vtx sIPQuadVtx[] = {
    /* 0 */ VTX(0, 0, 0, 0, 1 * IPQUAD_TEXH * (1 << 6), 0, 0, 0x7F, 255),
    /* 1 */ VTX(IPQUAD_W, 0, 0, 1 * IPQUAD_TEXW * (1 << 6), 1 * IPQUAD_TEXH * (1 << 6), 0, 0, 0x7F, 255),
    /* 2 */ VTX(IPQUAD_W, IPQUAD_H, 0, 1 * IPQUAD_TEXW * (1 << 6), 0, 0, 0, 0x7F, 255),
    /* 3 */ VTX(0, IPQUAD_H, 0, 0, 0, 0, 0, 0x7F, 255),
};

#define IPQUAD_TEX_SEGNUM 8

static Gfx sIPQuadDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(SEGMENT_ADDR(IPQUAD_TEX_SEGNUM, 0), G_IM_FMT_RGBA, G_IM_SIZ_16b, IPQUAD_TEXW, IPQUAD_TEXH, 0,
                         G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                         G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0,       //
                       0, 0, 0, 1,                //
                       COMBINED, 0, PRIMITIVE, 0, //
                       0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(sIPQuadVtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};

#define IPFB_W 64
#define IPFB_H 64

static u16* sIPFBTex[2];

static u16* sInnPaintingZBuffer;

static void* sMallocPtr = NULL;
void InnPainting_MallocBuffers(void) {
    assert(sMallocPtr == NULL);
    size_t sz = sizeof(u16[IPFB_W * IPFB_H]);
    size_t align = 64;
    sMallocPtr = ZeldaArena_MallocDebug(3 * sz + align);
    assert(sMallocPtr != NULL);
    uintptr_t p = (uintptr_t)sMallocPtr;
    p = (p + align - 1) & ALIGN_MASK(align);
    assert(sz % align == 0);
    sIPFBTex[0] = (u16*)p;
    p += sz;
    sIPFBTex[1] = (u16*)p;
    p += sz;
    sInnPaintingZBuffer = (u16*)p;
}
void InnPainting_FreeBuffers(void) {
    assert(sMallocPtr != NULL);
    ZeldaArena_FreeDebug(sMallocPtr);
    sMallocPtr = NULL;
}

static Gfx sInnPaintingSetupDL[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIA_PRIM2),
    gsDPSetOtherMode(G_AD_NOTPATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                     G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_SHADING_SMOOTH),
    gsSPEndDisplayList(),
};

static void ip_print_mtxf(const char* lhs_str, const MtxF* mf) {
    rmonPrintf("%s =\n"
               "/ %12.6f %12.6f %12.6f %12.6f \\\n"
               "| %12.6f %12.6f %12.6f %12.6f |\n"
               "| %12.6f %12.6f %12.6f %12.6f |\n"
               "\\ %12.6f %12.6f %12.6f %12.6f /\n",
               lhs_str, mf->xx, mf->xy, mf->xz, mf->xw, mf->yx, mf->yy, mf->yz, mf->yw, mf->zx, mf->zy, mf->zz, mf->zw,
               mf->wx, mf->wy, mf->wz, mf->ww);
}

static void ip_print_mtx(const char* lhs_str, const Mtx* mtx) {
    MtxF mf;

    Matrix_MtxToMtxF(mtx, &mf);
    ip_print_mtxf(lhs_str, &mf);
}

/**
 * @note the scissoring region and color image must be set before calling
 */
static void ip_rdp_fill_u16(Gfx* restrict* gfxP, size_t w, size_t h, u16 c) {
    Gfx* restrict gfx = *gfxP;

    gDPPipeSync(gfx++);
    gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
    gDPSetCycleType(gfx++, G_CYC_FILL);
    gDPSetFillColor(gfx++, (c << 16) | c);
    gDPFillRectangle(gfx++, 0, 0, w - 1, h - 1);

    *gfxP = gfx;
}

/**
 * restore buffers, scissor region, viewport, projView transform
 */
static void ip_restore_normal_drawing(Gfx* restrict* gfxP, PlayState* play) {
    Gfx* restrict gfx = *gfxP;
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, play->state.gfxCtx->curFrameBuffer);
    gDPSetDepthImage(gfx++, gZBuffer);

    // mimics View_ApplyLetterbox
    int varY = CLAMP_MAX(Letterbox_GetSize(), SCREEN_HEIGHT / 2);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0 + varY, SCREEN_WIDTH, SCREEN_HEIGHT - varY);

    Vp* vp = Graph_Alloc(gfxCtx, sizeof(Vp));
    Mtx* projMtx = Graph_Alloc(gfxCtx, sizeof(Mtx));
    *vp = play->view.vp;
    *projMtx = play->view.projection;

    gSPViewport(gfx++, vp);
    gSPPerspNormalize(gfx++, play->view.normal);
    gSPMatrix(gfx++, projMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gfx++, play->view.viewingPtr, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    *gfxP = gfx;
}

static void ip_draw_to_rgba16_texture(InnPaintingActor* this, PlayState* play, Gfx* restrict* gfxP,
                                      GraphicsContext* gfxCtx, u16* tex, u16* zBuffer, size_t w, size_t h, int timer) {
    Gfx* restrict gfx = *gfxP;
    f32 f;

    gDPNoOpString(gfx++, "ip_draw_to_rgba16_texture", 0);

    // idk
    gSPDisplayList(gfx++, sInnPaintingSetupDL);

    // clear zbuffer
    gDPPipeSync(gfx++);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, w, h);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, zBuffer);
    ip_rdp_fill_u16(&gfx, w, h, GPACK_ZDZ(G_MAXFBZ, 0));

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, w, tex);
    ip_rdp_fill_u16(&gfx, w, h, GPACK_RGBA5551(100, 100, 255, 1));
    gDPSetDepthImage(gfx++, zBuffer);

    Vp* viewport = Graph_Alloc(gfxCtx, sizeof(Vp));
    viewport->vp.vscale[0] = (w / 2) << 2;
    viewport->vp.vtrans[0] = (w / 2) << 2;
    viewport->vp.vscale[1] = (h / 2) << 2;
    viewport->vp.vtrans[1] = (h / 2) << 2;
    viewport->vp.vscale[2] = G_MAXZ / 2;
    viewport->vp.vtrans[2] = G_MAXZ / 2;
    gSPViewport(gfx++, viewport);

    Mtx* projMtx = (Mtx*)K0_TO_K1(Graph_Alloc(gfxCtx, sizeof(Mtx)));
    Mtx* lookAtMtx = (Mtx*)K0_TO_K1(Graph_Alloc(gfxCtx, sizeof(Mtx)));
    u16 perspNorm;

    guPerspective(projMtx, &perspNorm, 50.0f, (float)w / h, 0.1f, 10.0f, 1.0f);
    if (0) {
        ip_print_mtx("InnPainting_Draw projMtx", projMtx);
        rmonPrintf("perspNorm = %hu\n", perspNorm);
    }
    gSPPerspNormalize(gfx++, perspNorm);
    gSPMatrix(gfx++, projMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(lookAtMtx, 0.35f, 1.50f, 2.75f,             // eye
             0.20f, 1.30f, 1.78f,                        // at
             0.32f - 0.35f, 2.48f - 1.50f, 2.55f - 2.75f // up
    );
    if (0) {
        //
        ip_print_mtx("InnPainting_Draw lookAtMtx", lookAtMtx);
    }
    gSPMatrix(gfx++, lookAtMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    if (0) {
        MtxF projMtxF;
        MtxF lookAtMtxF;
        Matrix_MtxToMtxF(projMtx, &projMtxF);
        Matrix_MtxToMtxF(lookAtMtx, &lookAtMtxF);
        Matrix_Mult(&projMtxF, MTXMODE_NEW);
        Matrix_Mult(&lookAtMtxF, MTXMODE_APPLY);
        Vec3f va = { 0, 0, 0 }, vb;
        Matrix_MultVec3f(&va, &vb);
        rmonPrintf("%f %f %f -> %f %f %f\n", va.x, va.y, va.z, vb.x, vb.y, vb.z);
    }

    // TODO can't seem to make the directional light do anything
    // e.g. gdSPDefLights1(0, 255, 0, 255, 0, 0, 80, 80, 80)
    //      results in green only, no red anywhere
    static Lights1 lights = gdSPDefLights1(255, 255, 255, 0, 0, 0, 0, 0, 0);
    Lights1* lights_p = Graph_Alloc(gfxCtx, sizeof(lights));
    *lights_p = lights;
    gSPSetLights1(gfx++, (*lights_p));

    gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.slots[this->objectInnPaintingSlot].segment);
    gSPSegment(gfx++, 6, gSegments[6]);
    gDPNoOpString(gfx++, "draw gObjectInnPaintingBackgroundDL", 0);
    Matrix_Translate(0.0f, 0.0f, 0, MTXMODE_NEW);
    f = 0.001f;
    Matrix_Scale(f, f, f, MTXMODE_APPLY);
    gSPMatrix(gfx++, Matrix_NewMtx(gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(gfx++, gObjectInnPaintingBackgroundDL);

    gSegments[6] = VIRTUAL_TO_PHYSICAL(play->objectCtx.slots[this->objectPohSlot].segment);
    gSPSegment(gfx++, 6, gSegments[6]);
    gDPNoOpString(gfx++, "SkelAnime_Draw poeSkelAnime", 0);
    gfx = Gfx_SetupDL(gfx, SETUPDL_25);
    gDPSetEnvColor(gfx++, 255, 255, 255, 255);
    gSPSegment(gfx++, 8, gEmptyDL);
    Matrix_Translate(0.0f, 0.0f, 0, MTXMODE_NEW);
    f = 0.0003f;
    Matrix_Scale(f, f, f, MTXMODE_APPLY);
    gfx = SkelAnime_Draw(play, this->poeSkelAnime.skeleton, this->poeSkelAnime.jointTable, NULL, NULL, NULL, gfx);

    *gfxP = gfx;
}

void InnPainting_Draw(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    this->bufI = (this->bufI + 1) % 2;

    OPEN_DISPS(play->state.gfxCtx);

    // Draw to the texture
    {
        Gfx* restrict gfx = POLY_OPA_DISP;

        Matrix_Push();
        ip_draw_to_rgba16_texture(this, play, &gfx, play->state.gfxCtx, sIPFBTex[this->bufI], sInnPaintingZBuffer,
                                  IPFB_W, IPFB_H, this->timer);
        Matrix_Pop();

        ip_restore_normal_drawing(&gfx, play);

        POLY_OPA_DISP = gfx;
    }

    // ip_draw_to_rgba16_texture modifies the lights
    Lights_BindAndDraw(play, &this->actor.world.pos, play->roomCtx.curRoom.usePointLights);

    // Draw the painting in the world

    gSPDisplayList(POLY_OPA_DISP++, sInnPaintingSetupDL);

    gSPSetGeometryMode(POLY_OPA_DISP++, G_LIGHTING);
    if (play->roomCtx.curRoom.usePointLights)
        gSPSetGeometryMode(POLY_OPA_DISP++, G_LIGHTING_POSITIONAL);

    Matrix_SetTranslateRotateYXZ(XYZ(this->actor.world.pos), &this->actor.shape.rot);
    Matrix_Translate(-IPQUAD_W / 2, 4 * IPQUAD_H / 2, 0, MTXMODE_APPLY);
    for (int i = 0; i < 4; i++) {
        Matrix_Translate(0, -IPQUAD_H, 0, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPSegment(POLY_OPA_DISP++, IPQUAD_TEX_SEGNUM, sIPFBTex[this->bufI] + (i * IPQUAD_TEXW * IPQUAD_TEXH));
        gSPDisplayList(POLY_OPA_DISP++, sIPQuadDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
