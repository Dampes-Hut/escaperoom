#include "inn_painting.h"

#define FLAGS 0

void InnPainting_Init(Actor* thisx, PlayState* play);
void InnPainting_Destroy(Actor* thisx, PlayState* play);
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
    /**/ InnPainting_Update,
    /**/ InnPainting_Draw,
};

void InnPainting_Init(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    Actor_SetScale(thisx, 0.1f);
    InnPainting_MallocBuffers();
}

void InnPainting_Destroy(Actor* thisx, PlayState* play) {
    InnPainting_FreeBuffers();
}

void InnPainting_Update(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    this->timer++;
}

#define TEXW 32
#define TEXH 32

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
 */
static Vtx sInnPaintingVtx[] = {
    /* 0 */ VTX(-100, 0, 0, 0, 1 * TEXH * (1 << 6), 255, 255, 255, 255),
    /* 1 */ VTX(100, 0, 0, 1 * TEXW * (1 << 6), 1 * TEXH * (1 << 6), 255, 255, 255, 255),
    /* 2 */ VTX(100, 200, 0, 1 * TEXW * (1 << 6), 0, 255, 255, 255, 255),
    /* 3 */ VTX(-100, 200, 0, 0, 0, 255, 255, 255, 255),
};

static u16* sInnPaintingTex[2];

static u16* sInnPaintingZBuffer;

static u16* sInnPaintingTexRed;

static void* sMallocPtr = NULL;
void InnPainting_MallocBuffers(void) {
    assert(sMallocPtr == NULL);
    size_t sz = sizeof(u16[TEXW * TEXH]);
    size_t align = 64;
    sMallocPtr = ZeldaArena_MallocDebug(4 * sz + align);
    assert(sMallocPtr != NULL);
    uintptr_t p = (uintptr_t)sMallocPtr;
    p = (p + align - 1) & ALIGN_MASK(align);
    assert(sz % align == 0);
    sInnPaintingTex[0] = (u16*)p;
    p += sz;
    sInnPaintingTex[1] = (u16*)p;
    p += sz;
    sInnPaintingZBuffer = (u16*)p;
    p += sz;
    sInnPaintingTexRed = (u16*)p;
}
void InnPainting_FreeBuffers(void) {
    assert(sMallocPtr != NULL);
    ZeldaArena_FreeDebug(sMallocPtr);
    sMallocPtr = NULL;
}

#define INNPAINTINGTEXSEGNUM 8

static Gfx sInnPaintingDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(SEGMENT_ADDR(INNPAINTINGTEXSEGNUM, 0), G_IM_FMT_RGBA, G_IM_SIZ_16b, TEXW, TEXH, 0,
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
    gsSPVertex(sInnPaintingVtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};

static Gfx sInnPaintingFlatPrimColorDL[] = {
    gsDPPipeSync(),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_OFF),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE, //
                       0, 0, 0, 1,         //
                       0, 0, 0, COMBINED,  //
                       0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_PASS, G_RM_ZB_OPA_SURF2),
    gsSPVertex(sInnPaintingVtx, 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 0, 0),
    gsSPEndDisplayList(),
};

UNUSED
static void ip_cpu_fill_rgba16_texture_gradientxy(u16* tex, size_t w, size_t h) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            tex[y * w + x] = GPACK_RGBA5551(x * 255 / (w - 1), y * 255 / (h - 1), 0, 1);
        }
    }
}

static void ip_cpu_fill_rgba16_texture_solid(u16* tex, size_t w, size_t h, u16 c) {
    for (size_t y = 0; y < h; y++) {
        for (size_t x = 0; x < w; x++) {
            tex[y * w + x] = c;
        }
    }
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
static void ip_rdp_fill_u16(Gfx** gfxP, size_t w, size_t h, u16 c) {
    Gfx* gfx = *gfxP;

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
static void ip_restore_normal_drawing(Gfx** gfxP, PlayState* play) {
    Gfx* gfx = *gfxP;

    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, play->state.gfxCtx->curFrameBuffer);
    gDPSetDepthImage(gfx++, gZBuffer);

    // mimics View_ApplyLetterbox
    int varY = CLAMP_MAX(Letterbox_GetSize(), SCREEN_HEIGHT / 2);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0 + varY, SCREEN_WIDTH, SCREEN_HEIGHT - varY);

    // TODO unsure if play->view always has up-to-date stuff
    // yeah definitely looks odd, probably because of this
    gSPViewport(gfx++, &play->view.vp);
    gSPPerspNormalize(gfx++, play->view.normal);
    gSPMatrix(gfx++, &play->view.projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gfx++, &play->view.viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    *gfxP = gfx;
}

static void ip_draw_to_rgba16_texture(Gfx** gfxP, GraphicsContext* gfxCtx, u16* tex, u16* zBuffer, size_t w, size_t h,
                                      int timer) {
    Gfx* gfx = *gfxP;

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

    guPerspective(projMtx, &perspNorm, 80.0f, (float)w / h, 0.1f, 10.0f, 1.0f);
    if (0) {
        ip_print_mtx("InnPainting_Draw projMtx", projMtx);
        rmonPrintf("perspNorm = %hu\n", perspNorm);
    }
    gSPPerspNormalize(gfx++, perspNorm);
    gSPMatrix(gfx++, projMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

    guLookAt(lookAtMtx, 0, 0, 1, // eye
             0, 0, 0,            // at
             0.0f, 1.0f, 0.0f    // up
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

    gDPPipeSync(gfx++);
    Matrix_Mult(&gMtxFClear, MTXMODE_NEW);
    int framesPerTurnY = 30;
    Matrix_RotateY(2 * M_PI / framesPerTurnY * (timer % framesPerTurnY), MTXMODE_APPLY);
    // int framesPerTurnZ = 100;
    // Matrix_RotateZ(2 * M_PI / framesPerTurnZ * (timer % framesPerTurnZ), MTXMODE_APPLY);
    Matrix_Translate(0.0f, -0.5f, 0, MTXMODE_APPLY);
    f32 f = 0.005f;
    Matrix_Scale(f, f, f, MTXMODE_APPLY);
    gSPDisplayList(gfx++, sInnPaintingSetupDL);
    gSPMatrix(gfx++, Matrix_NewMtx(gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx++, 0, 0, 255, 100, 100, 255);
    gSPDisplayList(gfx++, sInnPaintingFlatPrimColorDL);
    Matrix_RotateY(M_PI, MTXMODE_APPLY);
    gSPMatrix(gfx++, Matrix_NewMtx(gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gDPSetPrimColor(gfx++, 0, 0, 100, 255, 100, 255);
    gSPDisplayList(gfx++, sInnPaintingFlatPrimColorDL);

    *gfxP = gfx;
}

void InnPainting_Draw(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    this->bufI = (this->bufI + 1) % 2;

    ip_cpu_fill_rgba16_texture_solid(sInnPaintingTexRed, TEXW, TEXH, GPACK_RGBA5551(255, 0, 0, 1));
    osWritebackDCache(sInnPaintingTexRed, sizeof(u16[TEXW * TEXH]));

    OPEN_DISPS(play->state.gfxCtx);

    // Draw to the texture
    {
        Gfx* gfx = POLY_OPA_DISP;

        Matrix_Push();
        ip_draw_to_rgba16_texture(&gfx, play->state.gfxCtx, sInnPaintingTex[this->bufI], sInnPaintingZBuffer, TEXW,
                                  TEXH, this->timer);
        Matrix_Pop();

        ip_restore_normal_drawing(&gfx, play);

        POLY_OPA_DISP = gfx;
    }

    // Draw the painting in the world
    gSPDisplayList(POLY_OPA_DISP++, sInnPaintingSetupDL);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPSegment(POLY_OPA_DISP++, INNPAINTINGTEXSEGNUM, sInnPaintingTex[this->bufI]);
    gSPDisplayList(POLY_OPA_DISP++, sInnPaintingDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
