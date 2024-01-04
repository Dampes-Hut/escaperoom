#include "chandelier_light.h"

#include "assets/objects/gameplay_keep/gameplay_keep.h"

#include "assets_mod/objects/object_chandelier/gChandelierDL.h"

#define FLAGS 0

void ChandelierLight_Init(Actor* thisx, PlayState* play);
void ChandelierLight_Destroy(Actor* thisx, PlayState* play);
void ChandelierLight_Update(Actor* thisx, PlayState* play);
void ChandelierLight_Draw(Actor* thisx, PlayState* play);

ActorInit ChandelierLight_InitVars = {
    /**/ ACTOR_INN_PAINTING,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_CHANDELIER,
    /**/ sizeof(ChandelierLightActor),
    /**/ ChandelierLight_Init,
    /**/ ChandelierLight_Destroy,
    /**/ ChandelierLight_Update,
    /**/ ChandelierLight_Draw,
};

void ChandelierLight_Init(Actor* thisx, PlayState* play) {
    ChandelierLightActor* this = (ChandelierLightActor*)thisx;

#define ACTOR_SCALE 0.01f
    Actor_SetScale(&this->actor, ACTOR_SCALE);

    Lights_PointSetInfo(&this->lightInfo, XYZ(this->actor.world.pos), 255, 220, 150, 400, LIGHT_POINT_NOGLOW);
    this->lightInfo.params.point.y -= 35;

    this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);
    assert(this->lightNode != NULL);
    if (this->lightNode == NULL) {
        Actor_Kill(&this->actor);
        return;
    }

    static_assert(NUM_CANDLES == 6, "If the model changes check this");
    // The model is made of 3 planes, each has 2 candles.
    // The XY plane has the two candles at the candleFlamePositions coordinates
    Vec3f candleFlamePositions[2] = {
        // world coordinates relative to the actor
        { 23.1f, -31.7f, 0.0f },
        { -24.2f, -32.8f, 0.0f },
    };
    for (int iCandle = 0; iCandle < NUM_CANDLES; iCandle++) {
        ChandelierCandleInfo* candle = &this->candles[iCandle];
        int iPlane = iCandle / 2;
        Vec3f* flamePos = &candleFlamePositions[iCandle % 2];

        Matrix_Translate(XYZ(this->actor.world.pos), MTXMODE_NEW);
        Matrix_RotateY(iPlane * 2 * M_PI / NUM_CANDLES, MTXMODE_APPLY);
        Matrix_MultVec3f(flamePos, &candle->flamePos);

        candle->brightness = Rand_ZeroOne();
    }
}

void ChandelierLight_Destroy(Actor* thisx, PlayState* play) {
    ChandelierLightActor* this = (ChandelierLightActor*)thisx;

    if (this->lightNode != NULL) {
        LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
    }
}

void ChandelierLight_Update(Actor* thisx, PlayState* play) {
    ChandelierLightActor* this = (ChandelierLightActor*)thisx;

    this->timer++;

    float sumBrightness = 0.0f;

    for (int iCandle = 0; iCandle < NUM_CANDLES; iCandle++) {
        ChandelierCandleInfo* candle = &this->candles[iCandle];

        float targetBrightness = Rand_ZeroOne();
        candle->brightness = LERP(candle->brightness, targetBrightness, 0.8f);

        sumBrightness += candle->brightness;
    }

    float avgBrightness = sumBrightness / NUM_CANDLES;

    /*
     * candle->brightness follows Uniform(0,1)
     * it has mean=0.5 stdev=sqrt(1/12)~=0.29
     *
     * hence per the central limit theorem,
     * avgBrightness approximately follows Normal(mean,stdev)
     *
     * we scale avgBrightness here (and clamp to the (0;1) interval)
     * so it is more likely to take a wider range of values.
     * The scaling is done by 1.3*stdev,
     * which corresponds to mapping the 81%-likely center values to 0-1
     * (Wolfram Alpha query: integrate 2 * 1/sqrt(2pi)*e^(-x^2/2) from 0 to 1.3)
     */
    static const float mean = 0.5f, stdev = sqrt(1.0f / 12.0f);
    float chandelierBrightness = (avgBrightness - mean) / (1.3f * stdev) + mean;
    chandelierBrightness = CLAMP(chandelierBrightness, 0.0f, 1.0f);
    this->brightness = LERP(this->brightness, chandelierBrightness, 0.8f);

    LightPoint* lp = &this->lightInfo.params.point;
    lp->radius = LERP(400, 450, this->brightness);
}

// based on z_lights.c sGlowSetupDL, but with zbuffering
static Gfx sGlowSetupDL[] = {
    gsDPPipeSync(),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPSetCombineLERP(0, 0, 0, PRIMITIVE,      //
                       TEXEL0, 0, PRIMITIVE, 0, //
                       0, 0, 0, PRIMITIVE,      //
                       TEXEL0, 0, PRIMITIVE, 0),
    gsDPSetOtherMode(G_AD_PATTERN | G_CD_NOISE | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE |
                         G_TD_CLAMP | G_TP_PERSP | G_CYC_1CYCLE | G_PM_NPRIMITIVE,
                     G_AC_THRESHOLD | G_ZS_PIXEL | G_RM_ZB_CLD_SURF | G_RM_ZB_CLD_SURF2),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_SHADING_SMOOTH),
    gsSPBranchList(gGlowCircleTextureLoadDL),
};

/**
 * @param size in units
 * @note sGlowSetupDL should have been called already
 * @note modifies the current matrix
 */
static void cl_draw_glow(Gfx* restrict* gfxP, GraphicsContext* gfxCtx, float size, Color_RGBA8 color) {
    Gfx* restrict gfx = *gfxP;
    f32 scale = size / 1000.0f;
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);
    gSPMatrix(gfx++, Matrix_NewMtx(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(gfx++, 0, 0, color.r, color.g, color.b, color.a);
    gSPDisplayList(gfx++, gGlowCircleDL);
    *gfxP = gfx;
}

void ChandelierLight_Draw(Actor* thisx, PlayState* play) {
    ChandelierLightActor* this = (ChandelierLightActor*)thisx;
    LightPoint* lp = &this->lightInfo.params.point;

    OPEN_DISPS(play->state.gfxCtx);

    u8* color = lp->color;
    float i = LERP(0.85f, 1.0f, this->brightness);
    gDPSetEnvColor(POLY_OPA_DISP++, i * color[0], i * color[1], i * color[2], 255);
    Gfx_DrawDListOpa(play, gChandelierDL);

    gDPSetBlendColor(POLY_XLU_DISP++, 0, 0, 0, 20); // for sGlowSetupDL's G_AC_THRESHOLD
    gSPDisplayList(POLY_XLU_DISP++, sGlowSetupDL);

    for (int iCandle = 0; iCandle < NUM_CANDLES; iCandle++) {
        ChandelierCandleInfo* candle = &this->candles[iCandle];

        Matrix_Translate(XYZ(candle->flamePos), MTXMODE_NEW);
        cl_draw_glow(&POLY_XLU_DISP, play->state.gfxCtx, LERP(12.0f, 17.0f, candle->brightness),
                     (Color_RGBA8){ color[0], color[1], color[2], LERP(40, 60, candle->brightness) });
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
