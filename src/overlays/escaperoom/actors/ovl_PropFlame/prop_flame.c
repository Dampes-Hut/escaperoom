#include "prop_flame.h"

#include "assets/objects/gameplay_keep/gameplay_keep.h"

#define FLAGS 0

void PropFlame_Init(Actor* thisx, PlayState* play);
void PropFlame_Destroy(Actor* thisx, PlayState* play);
void PropFlame_Update(Actor* thisx, PlayState* play);
void PropFlame_Draw(Actor* thisx, PlayState* play);

ActorInit PropFlame_InitVars = {
    /**/ ACTOR_PROP_FLAME,
    /**/ ACTORCAT_MISC,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(PropFlameActor),
    /**/ PropFlame_Init,
    /**/ PropFlame_Destroy,
    /**/ PropFlame_Update,
    /**/ PropFlame_Draw,
};

void PropFlame_Init(Actor* thisx, PlayState* play) {
    PropFlameActor* this = (PropFlameActor*)thisx;
}

void PropFlame_Destroy(Actor* thisx, PlayState* play) {
    PropFlameActor* this = (PropFlameActor*)thisx;
}

void PropFlame_Update(Actor* thisx, PlayState* play) {
    PropFlameActor* this = (PropFlameActor*)thisx;
}

void PropFlame_Draw(Actor* thisx, PlayState* play) {
    PropFlameActor* this = (PropFlameActor*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    // much copied from ObjSyokudai_Draw

    f32 flameScale = 0.027f * (1 + PROP_FLAME_SIZE(&this->actor)) / 2.0f;

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x40, 1, 0,
                                (play->gameplayFrames * -20) & 0x1FF, 0x20, 0x80));

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 200, 50, 255);

    gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 50, 0);

    Matrix_RotateY(BINANG_TO_RAD((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)) - this->actor.shape.rot.y + 0x8000)),
                   MTXMODE_APPLY);
    Matrix_Scale(flameScale, flameScale, flameScale, MTXMODE_APPLY);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);

    CLOSE_DISPS(play->state.gfxCtx);
}
