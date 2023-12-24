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

void InnPainting_Draw(Actor* thisx, PlayState* play) {
    InnPaintingActor* this = (InnPaintingActor*)thisx;

    this->bufI = (this->bufI + 1) % 2;

    for (int y = 0; y < TEXH; y++) {
        for (int x = 0; x < TEXW; x++) {
            sInnPaintingTex[this->bufI][y * TEXW + x] =
                GPACK_RGBA5551(x * 255 / (TEXW - 1), y * 255 / (TEXH - 1), 0, 1);

            sInnPaintingTexRed[y * TEXW + x] = GPACK_RGBA5551(255, 0, 0, 1);
        }
    }
    osWritebackDCache(sInnPaintingTex[this->bufI], sizeof(sInnPaintingTex[this->bufI]));

    OPEN_DISPS(play->state.gfxCtx);

    // idk
    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPClearGeometryMode(POLY_OPA_DISP++, G_LIGHTING);

    Gfx* gfx = POLY_OPA_DISP;

    gDPPipeSync(gfx++);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, TEXW, TEXH);
    // clear zbuffer
    {
        gDPPipeSync(gfx++);
        gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, TEXW, sInnPaintingZBuffer);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        u16 c = GPACK_ZDZ(G_MAXFBZ, 0);
        gDPSetFillColor(gfx++, (c << 16) | c);
        gDPFillRectangle(gfx++, 0, 0, TEXW - 1, TEXH - 1);
    }
    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, TEXW, sInnPaintingTex[this->bufI]);
    gDPSetDepthImage(gfx++, sInnPaintingZBuffer);
    static Vp viewport = { (TEXW / 2) << 2, (TEXH / 2) << 2, G_MAXZ / 2, 0,
                           (TEXW / 2) << 2, (TEXH / 2) << 2, G_MAXZ / 2, 0 };
    gSPViewport(gfx++, &viewport);
    {
        if (0) {
            gSPPerspNormalize(gfx++, 2);
            gSPMatrix(gfx++, &gMtxClear, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
        }

        // if (0)
        {
            Mtx* projMtx = Graph_Alloc(play->state.gfxCtx, sizeof(Mtx));
            Mtx* lookAtMtx = Graph_Alloc(play->state.gfxCtx, sizeof(Mtx));
            u16 perspNorm;

            guPerspective(projMtx, &perspNorm, 80.0f, (float)TEXW / TEXH, 0.1f, 10.0f, 1.0f);
            // guOrtho(projMtx, -100, 100, 100, -100, 1, 1000, 1);
            MtxF projMtxF;
            Matrix_MtxToMtxF(projMtx, &projMtxF);
            MtxF* mf = &projMtxF;
            if (0) {
                rmonPrintf("InnPainting_Draw projMtxF =\n"
                           "/ %12.6f %12.6f %12.6f %12.6f \\\n"
                           "| %12.6f %12.6f %12.6f %12.6f |\n"
                           "| %12.6f %12.6f %12.6f %12.6f |\n"
                           "\\ %12.6f %12.6f %12.6f %12.6f /\n",
                           mf->xx, mf->xy, mf->xz, mf->xw, mf->yx, mf->yy, mf->yz, mf->yw, mf->zx, mf->zy, mf->zz,
                           mf->zw, mf->wx, mf->wy, mf->wz, mf->ww);
                rmonPrintf("perspNorm = %hu\n", perspNorm);
            }

            gSPPerspNormalize(gfx++, perspNorm);
            gSPMatrix(gfx++, projMtx, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);

            guLookAt(lookAtMtx, 0, 0, 1, // eye
                     0, 0, 0,            // at
                     0.0f, 1.0f, 0.0f    // up
            );

            Matrix_MtxToMtxF(lookAtMtx, &projMtxF);
            mf = &projMtxF;
            if (0) {
                rmonPrintf("InnPainting_Draw lookAtMtx =\n"
                           "/ %12.6f %12.6f %12.6f %12.6f \\\n"
                           "| %12.6f %12.6f %12.6f %12.6f |\n"
                           "| %12.6f %12.6f %12.6f %12.6f |\n"
                           "\\ %12.6f %12.6f %12.6f %12.6f /\n",
                           mf->xx, mf->xy, mf->xz, mf->xw, mf->yx, mf->yy, mf->yz, mf->yw, mf->zx, mf->zy, mf->zz,
                           mf->zw, mf->wx, mf->wy, mf->wz, mf->ww);
            }

            gSPMatrix(gfx++, lookAtMtx, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);
        }
    }
    Matrix_Push();
    {
        u16 c = GPACK_RGBA5551(0, 0, 255, 1);
        gDPSetRenderMode(gfx++, G_RM_NOOP, G_RM_NOOP2);
        gDPSetCycleType(gfx++, G_CYC_FILL);
        gDPSetFillColor(gfx++, (c << 16) | c);
        gDPFillRectangle(gfx++, 0, 0, TEXW - 1, TEXH - 1);
    }
    // if (0)
    {
        Matrix_Mult(&gMtxFClear, MTXMODE_NEW);
        // int framesPerTurnY = 30;
        // Matrix_RotateY(2 * M_PI / framesPerTurnY * (this->timer % framesPerTurnY), MTXMODE_APPLY);
        // int framesPerTurnZ = 100;
        // Matrix_RotateZ(2 * M_PI / framesPerTurnZ * (this->timer % framesPerTurnZ), MTXMODE_APPLY);
        Matrix_Translate(0.0f, -1.0f, 0, MTXMODE_APPLY);
        f32 f = 0.02f;
        Matrix_Scale(f, f, f, MTXMODE_APPLY);
        gfx = Gfx_SetupDL(gfx, SETUPDL_25);
        gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
        gSPMatrix(gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gDPSetPrimColor(gfx++, 0, 0, 255, 100, 100, 255);
        gSPDisplayList(gfx++, sInnPaintingFlatPrimColorDL);
    }
    if (0) {
        Matrix_Mult(&gMtxFClear, MTXMODE_NEW);
        Matrix_Translate(0, -10, 0, MTXMODE_APPLY);
        Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);
        gfx = Gfx_SetupDL(gfx, SETUPDL_25);
        gSPClearGeometryMode(POLY_OPA_DISP++, G_LIGHTING);
        gSPMatrix(gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gSPSegment(gfx++, INNPAINTINGTEXSEGNUM, sInnPaintingTexRed);
        gSPClearGeometryMode(gfx++, G_LIGHTING | G_CULL_BOTH);
        gSPDisplayList(gfx++, sInnPaintingDL);
    }
    Matrix_Pop();
    // restore buffers, scissor region, viewport
    gDPPipeSync(gfx++);
    gDPSetColorImage(gfx++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, play->state.gfxCtx->curFrameBuffer);
    gDPSetDepthImage(gfx++, gZBuffer);
    gDPSetScissor(gfx++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT); // TODO rip letterboxing?
    // TODO unsure if play->view always has up-to-date stuff
    // yeah definitely looks odd, probably because of this
    gSPViewport(gfx++, &play->view.vp);
    gSPPerspNormalize(gfx++, play->view.normal);
    gSPMatrix(gfx++, &play->view.projection, G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_PROJECTION);
    gSPMatrix(gfx++, &play->view.viewing, G_MTX_NOPUSH | G_MTX_MUL | G_MTX_PROJECTION);

    POLY_OPA_DISP = gfx;

    Gfx_SetupDL_25Opa(play->state.gfxCtx);
    gSPClearGeometryMode(POLY_OPA_DISP++, G_LIGHTING);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPSegment(POLY_OPA_DISP++, INNPAINTINGTEXSEGNUM, sInnPaintingTex[this->bufI]);
    gSPDisplayList(POLY_OPA_DISP++, sInnPaintingDL);

    if (0) {
        Matrix_Translate(50, 50, 50, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 200, 200, 100, 255);
        gSPDisplayList(POLY_OPA_DISP++, sInnPaintingFlatPrimColorDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
