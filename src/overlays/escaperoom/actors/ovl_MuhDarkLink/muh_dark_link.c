// SPDX-FileCopyrightText: 2024 Dragorn421
// SPDX-License-Identifier: CC0-1.0

#include "muh_dark_link.h"

#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_torch2/object_torch2.h"

#define FLAGS (ACTOR_FLAG_NOCULL_UPDATE | ACTOR_FLAG_NOCULL_DRAW)

void MuhDarkLink_Init(Actor* thisx, PlayState* play);
void MuhDarkLink_Destroy(Actor* thisx, PlayState* play);
void MuhDarkLink_Update(Actor* thisx, PlayState* play);
void MuhDarkLink_Draw(Actor* thisx, PlayState* play);

static void muhdl_fadeaction_noop(MuhDarkLinkActor* this, PlayState* play);
static void muhdl_fadeaction_visible(MuhDarkLinkActor* this, PlayState* play);
static void muhdl_fadeaction_fade_out(MuhDarkLinkActor* this, PlayState* play);
static void muhdl_fadeaction_hidden_cooldown(MuhDarkLinkActor* this, PlayState* play);
static void muhdl_fadeaction_hidden(MuhDarkLinkActor* this, PlayState* play);
static void muhdl_fadeaction_fade_in(MuhDarkLinkActor* this, PlayState* play);
static void muhdl_fadeaction_visible_cooldown(MuhDarkLinkActor* this, PlayState* play);

ActorInit MuhDarkLink_InitVars = {
    /**/ ACTOR_MUH_DARK_LINK,
    /**/ ACTORCAT_BOSS,
    /**/ FLAGS,
    /**/ OBJECT_TORCH2,
    /**/ sizeof(MuhDarkLinkActor),
    /**/ MuhDarkLink_Init,
    /**/ MuhDarkLink_Destroy,
    /**/ MuhDarkLink_Update,
    /**/ MuhDarkLink_Draw,
};

static ColliderCylinderInit sColliderCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xFFCFFFFF, 0x00, 0x00 },
        TOUCH_NONE,
        BUMP_ON,
        OCELEM_ON,
    },
    { 30, 60, 0, { 0, 0, 0 } },
};

void MuhDarkLink_Init(Actor* thisx, PlayState* play) {
    MuhDarkLinkActor* this = (MuhDarkLinkActor*)thisx;

    if (Flags_GetClear(play, play->roomCtx.curRoom.num)) {
        Actor_Kill(&this->actor);
        return;
    }

    this->csState = MUHDL_CS_WAIT;

    this->centerX = this->actor.home.pos.x;
    this->centerZ = this->actor.home.pos.z;

    Collider_InitAndSetCylinder(play, &this->colliderCylinder, &this->actor, &sColliderCylinderInit);

    SkelAnime_InitLink(play, &this->skelAnime, &gDarkLinkSkel, &gPlayerAnim_link_normal_wait, 9, NULL, NULL, 0);
    assert(this->skelAnime.jointTable != NULL);

    this->eyesColor = (Color_RGB8){ 200, 69, 20 };

    this->minSmokeIntensity = 0.1f;
    this->maxSmokeIntensity = 1.0f;
    this->smokeIntensity = this->minSmokeIntensity;

    this->minBodyAlpha = 0;
    this->maxBodyAlpha = 255;
    this->bodyAlpha = this->maxBodyAlpha;

    this->fadeActionFunc = muhdl_fadeaction_noop;
}

void MuhDarkLink_Destroy(Actor* thisx, PlayState* play) {
    MuhDarkLinkActor* this = (MuhDarkLinkActor*)thisx;

    Collider_DestroyCylinder(play, &this->colliderCylinder);
}

static void muhdl_copy_player_pose(MuhDarkLinkActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    memcpy(this->skelAnime.jointTable, player->skelAnime.jointTable, sizeof(Vec3s[PLAYER_LIMB_MAX]));
}

static void muhdl_update_pos(MuhDarkLinkActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Vec3s* thisRot = &this->actor.shape.rot;
    Vec3s* playerRot = &player->actor.shape.rot;

    Vec3f* thisPos = &this->actor.world.pos;
    Vec3f* playerPos = &player->actor.world.pos;

    thisRot->y = (s16)(playerRot->y + 0x8000);

    this->actor.gravity = player->actor.gravity;
    Actor_MoveXZGravity(&this->actor); // handle gravity

    thisPos->x = 2.0f * this->centerX - playerPos->x;
    thisPos->z = 2.0f * this->centerZ - playerPos->z;
    Actor_UpdateBgCheckInfo(play, &this->actor, 26.0f, player->ageProperties->wallCheckRadius,
                            player->ageProperties->ceilingCheckHeight,
                            UPDBGCHECKINFO_FLAG_0 | UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 |
                                UPDBGCHECKINFO_FLAG_3 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_5);
    playerPos->x = 2.0f * this->centerX - thisPos->x;
    playerPos->z = 2.0f * this->centerZ - thisPos->z;
}

#define FADE_OUT_DIST_THRESHOLD 30
#define FADE_IN_DIST_THRESHOLD 40

static void muhdl_set_fadeaction(MuhDarkLinkActor* this, MuhDarkLinkActorFadeActionFunc fadeActionFunc) {
    this->fadeActionFunc = fadeActionFunc;
    this->fadeTimer = 0;
}
static void muhdl_fadeaction_noop(MuhDarkLinkActor* this, PlayState* play) {
    // no-op for manual control
}
static void muhdl_set_fade_factor(MuhDarkLinkActor* this, float f) {
    ASSERT_SOFT(f >= 0.0f && f <= 1.0f);
    f = CLAMP(f, 0.0f, 1.0f);
    this->smokeIntensity = LERP(this->minSmokeIntensity, this->maxSmokeIntensity, f);
    this->bodyAlpha = LERP(this->maxBodyAlpha, this->minBodyAlpha, f);
}
static void muhdl_fadeaction_visible(MuhDarkLinkActor* this, PlayState* play) {
    muhdl_set_fade_factor(this, 0.0f);
    if (this->actor.xyzDistToPlayerSq < SQ(FADE_OUT_DIST_THRESHOLD) || this->colliderCylinder.base.acFlags & AC_HIT) {
        muhdl_set_fadeaction(this, muhdl_fadeaction_fade_out);
    }
}
static void muhdl_fadeaction_fade_out(MuhDarkLinkActor* this, PlayState* play) {
    this->fadeTimer++;
#define FADE_OUT_TIME 5
    muhdl_set_fade_factor(this, (float)this->fadeTimer / FADE_OUT_TIME);
    if (this->fadeTimer >= FADE_OUT_TIME) {
        muhdl_set_fadeaction(this, muhdl_fadeaction_hidden_cooldown);
    }
}
static void muhdl_fadeaction_hidden_cooldown(MuhDarkLinkActor* this, PlayState* play) {
    this->fadeTimer++;
#define HIDDEN_COOLDOWN_TIME 5
    muhdl_set_fade_factor(this, 1.0f);
    if (this->fadeTimer >= HIDDEN_COOLDOWN_TIME) {
        muhdl_set_fadeaction(this, muhdl_fadeaction_hidden);
    }
}
static void muhdl_fadeaction_hidden(MuhDarkLinkActor* this, PlayState* play) {
    muhdl_set_fade_factor(this, 1.0f);
    if (this->actor.xyzDistToPlayerSq > SQ(FADE_IN_DIST_THRESHOLD)) {
        muhdl_set_fadeaction(this, muhdl_fadeaction_fade_in);
    }
}
static void muhdl_fadeaction_fade_in(MuhDarkLinkActor* this, PlayState* play) {
    this->fadeTimer++;
#define FADE_IN_TIME 5
    muhdl_set_fade_factor(this, 1.0f - (float)this->fadeTimer / FADE_IN_TIME);
    if (this->fadeTimer >= FADE_IN_TIME) {
        muhdl_set_fadeaction(this, muhdl_fadeaction_visible_cooldown);
    }
}
static void muhdl_fadeaction_visible_cooldown(MuhDarkLinkActor* this, PlayState* play) {
    this->fadeTimer++;
#define VISIBLE_COOLDOWN_TIME 5
    muhdl_set_fade_factor(this, 0.0f);
    if (this->fadeTimer >= VISIBLE_COOLDOWN_TIME) {
        muhdl_set_fadeaction(this, muhdl_fadeaction_visible);
    }
}

static void muhdl_update_effects(MuhDarkLinkActor* this, PlayState* play) {
    int targetActiveSmokeEffects = ROUND(this->smokeIntensity * ARRAY_COUNT(this->smokeEffects));

    for (int i = 0; i < ARRAY_COUNT(this->smokeEffects); i++) {
        MuhDarkLinkActorSmokeEffect* eff = &this->smokeEffects[i];

        if (eff->duration == 0) {
#define EFF_DURATION_MIN 40
#define EFF_DURATION_MAX 60
            // to fill a smokeIntensity proportion of smokeEffects
            if (targetActiveSmokeEffects > this->nActiveSmokeEffects) {
                this->nActiveSmokeEffects++;
                eff->limb = (u8)(1 + Rand_ZeroFloat(PLAYER_LIMB_MAX - 1));
                eff->duration = (u8)(EFF_DURATION_MIN + Rand_ZeroFloat(EFF_DURATION_MAX - EFF_DURATION_MIN));
                eff->timer = 0;
                float f = 20.0f;
                eff->posOffset.x = Rand_CenteredFloat(f);
                eff->posOffset.y = Rand_CenteredFloat(f);
                eff->posOffset.z = Rand_CenteredFloat(f);
            }
        }
        if (eff->duration != 0) {
            assert(eff->timer < eff->duration);
            // fade in and out
            // t                texIndex
            // EFF_DURATION     7
            // ~EFF_DURATION/2  0
            // 0                7
            eff->texIndex = 7 * SQ((float)eff->timer / eff->duration * 2 - 1);

            eff->timer++;

            if (eff->timer == eff->duration) {
                eff->duration = 0;
                this->nActiveSmokeEffects--;
            }
        }
    }
}

static void muhdl_update_debug(MuhDarkLinkActor* this, PlayState* play) {
    if (play->state.input[0].press.button & BTN_L) {
        Player* player = GET_PLAYER(play);

        // this->centerX = player->actor.world.pos.x;
        // this->centerZ = player->actor.world.pos.z;
    }
}

static void muhdl_setup_intro_cs(MuhDarkLinkActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->csState = MUHDL_CS_INTRO;
    Cutscene_StartManual(play, &play->csCtx);
    this->subCamId = Play_CreateSubCamera(play);
    Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
    Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);

    this->subCamAt = this->actor.world.pos;
    this->subCamAt.y += 50.0f;
    this->subCamEye = player->actor.world.pos;
    this->subCamEye.y += 100.0f;

    muhdl_set_fadeaction(this, muhdl_fadeaction_noop);
    this->bodyAlpha = 0;
    this->smokeIntensity = 0.0f;

    this->csTimer = 0;
    this->csSubState = 0;
}

static void muhdl_end_intro_cs(MuhDarkLinkActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->csState = MUHDL_CS_NONE;

    Cutscene_StopManual(play, &play->csCtx);

    Message_StartTextbox(play, 0x300, NULL);

    assert(this->subCamId != SUB_CAM_ID_DONE);
    if (this->subCamId != SUB_CAM_ID_DONE) {
        Play_ReturnToMainCam(play, this->subCamId, 0);
        this->subCamId = SUB_CAM_ID_DONE;
    }
}

static void muhdl_update_intro_cs(MuhDarkLinkActor* this, PlayState* play) {
    this->subCamAt = this->actor.world.pos;
    this->subCamAt.y += 40.0f;
    Math_Vec3f_Sum(&this->subCamAt, &(Vec3f){ 80.0f, 20.0f, 30.0f }, &this->subCamEye);

    Play_SetCameraAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);

    this->csTimer++;

    switch (this->csSubState) {
        case 0:
            this->doDraw = true;

            Math_StepToF(&this->smokeIntensity, 1.0f, 0.04f);
            if (this->csTimer > 80) {
                this->csTimer = 0;
                this->csSubState++;
            }
            break;

        case 1:
            if (this->bodyAlpha <= 255 - 6) {
                this->bodyAlpha += 6;
            } else {
                this->bodyAlpha = 255;
            }

            if (this->bodyAlpha > 100) {
                Math_StepToF(&this->smokeIntensity, 0.0f, 0.04f);
            }

            if (this->csTimer > 100) {
                muhdl_set_fadeaction(this, muhdl_fadeaction_visible);
                muhdl_end_intro_cs(this, play);
            }
            break;
    }
}

static void muhdl_setup_ded_cs(MuhDarkLinkActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    this->csState = MUHDL_CS_DED;
    Cutscene_StartManual(play, &play->csCtx);
    this->subCamId = Play_CreateSubCamera(play);
    Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
    Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);

    this->subCamAt = this->actor.world.pos;
    this->subCamAt.y += 50.0f;
    this->subCamEye = player->actor.world.pos;
    this->subCamEye.y += 100.0f;

    muhdl_set_fadeaction(this, muhdl_fadeaction_noop);
    this->bodyAlpha = 255;
    this->smokeIntensity = 0.0f;

    this->csTimer = 0;
    this->csSubState = 0;
}

static void muhdl_end_ded_cs(MuhDarkLinkActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    Flags_SetClear(play, play->roomCtx.curRoom.num);
    Actor_Kill(&this->actor);

    Cutscene_StopManual(play, &play->csCtx);

    assert(this->subCamId != SUB_CAM_ID_DONE);
    if (this->subCamId != SUB_CAM_ID_DONE) {
        Play_ReturnToMainCam(play, this->subCamId, 0);
        this->subCamId = SUB_CAM_ID_DONE;
    }
}

static void muhdl_update_ded_cs(MuhDarkLinkActor* this, PlayState* play) {
    rmonPrintf("muhdl_update_ded_cs this->csSubState=%d\n", this->csSubState);
    this->subCamAt = this->actor.world.pos;
    this->subCamAt.y += 40.0f;
    Math_Vec3f_Sum(&this->subCamAt, &(Vec3f){ -30.0f, 20.0f, 80.0f }, &this->subCamEye);

    Play_SetCameraAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);

    this->csTimer++;

    switch (this->csSubState) {
        case 0:
            rmonPrintf("muhdl_update_ded_cs 0\n");
            Math_StepToF(&this->smokeIntensity, 1.0f, 0.04f);
            if (this->bodyAlpha > 2) {
                this->bodyAlpha -= 2;
            }

            if (this->csTimer == 30) {
                Sfx_PlaySfxAtPos(&this->actor.projectedPos, NA_SE_PL_DAMAGE);
            }

            if (this->csTimer > 80) {
                this->csTimer = 0;
                this->csSubState++;
            }
            break;

        case 1:
            rmonPrintf("muhdl_update_ded_cs 1\n");
            if (this->bodyAlpha > 0) {
                this->bodyAlpha--;
            }
            Math_StepToF(&this->smokeIntensity, 0.0f, 0.015f);

            if (this->csTimer > 100) {
                Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
                muhdl_end_ded_cs(this, play);
            }
            break;
    }
}

void MuhDarkLink_Update(Actor* thisx, PlayState* play) {
    MuhDarkLinkActor* this = (MuhDarkLinkActor*)thisx;

#ifndef NDEBUG
    muhdl_update_debug(this, play);
#endif

    if (this->csState == MUHDL_CS_WAIT) {
        Player* player = GET_PLAYER(play);

        if (player->actor.world.pos.y < this->actor.home.pos.y + 5.0f) {
            this->actor.world.pos.x = 2.0f * this->centerX - player->actor.world.pos.x;
            this->actor.world.pos.z = 2.0f * this->centerZ - player->actor.world.pos.z;
            this->actor.prevPos = this->actor.world.pos;

            muhdl_setup_intro_cs(this, play);
        } else {
            return;
        }
    }

    muhdl_copy_player_pose(this, play);
    muhdl_update_pos(this, play);

    muhdl_update_effects(this, play);

    if (this->fadeActionFunc != NULL) {
        this->fadeActionFunc(this, play);
    }

    Collider_UpdateCylinder(&this->actor, &this->colliderCylinder);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderCylinder.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderCylinder.base);

    if (this->csState == MUHDL_CS_INTRO) {
        muhdl_update_intro_cs(this, play);
    }

    if (this->csState == MUHDL_CS_NONE &&
        Actor_FindNearby(play, &this->actor, ACTOR_PROP_FLAME, ACTORCAT_MISC, 20.0f)) {

        muhdl_setup_ded_cs(this, play);
    }

    if (this->csState == MUHDL_CS_DED) {
        muhdl_update_ded_cs(this, play);
    }
}

static void muhdl_draw_debug_impl(MuhDarkLinkActor* this, PlayState* play, GfxPrint* printer) {
    GfxPrint_SetColor(printer, 255, 0, 255, 255);
    GfxPrint_Printf(printer, "bodyAlpha=%d\n", (int)this->bodyAlpha);
    GfxPrint_Printf(printer, "smokeIntensity=%f\n", this->smokeIntensity);
    GfxPrint_Printf(printer, "csState=%d\n", this->csState);
    GfxPrint_Printf(printer, "csTimer=%d\n", this->csTimer);
    GfxPrint_Printf(printer, "csSubState=%d\n", this->csSubState);
}

static void muhdl_draw_debug(MuhDarkLinkActor* this, PlayState* play) {
    GfxPrint printer;
    Gfx* restrict gfx;

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_OPA_DISP + 1;
    gSPDisplayList(OVERLAY_DISP++, gfx);

    GfxPrint_Init(&printer);
    GfxPrint_Open(&printer, gfx);

    GfxPrint_SetBasePosPx(&printer, 10, 60);
    GfxPrint_SetPosPx(&printer, 0, 0);

    muhdl_draw_debug_impl(this, play, &printer);

    gfx = GfxPrint_Close(&printer);
    GfxPrint_Destroy(&printer);

    gSPEndDisplayList(gfx++);
    gSPBranchList(POLY_OPA_DISP, gfx);
    POLY_OPA_DISP = gfx;

    CLOSE_DISPS(play->state.gfxCtx);
}

void muhdl_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, void* arg, Gfx** gfx) {
    MuhDarkLinkActor* this = arg;

    assert(limbIndex < PLAYER_LIMB_MAX);

    this->bodyPartsPos[limbIndex] = Matrix_GetTranslation();
}

Gfx* func_8002E830(Vec3f* object, Vec3f* eye, Vec3f* lightDir, GraphicsContext* gfxCtx, Gfx* gfx, Hilite** hilite);

static void muhdl_draw_dark_link(MuhDarkLinkActor* this, PlayState* play) {
    bool isTranslucent;
    Gfx* restrict* gfxP;
    Gfx* restrict gfx;
    Vec3f lightDir;
    Hilite* hilite;

    // also draw if this->bodyAlpha == 0 to update bodyPartsPos

    isTranslucent = this->bodyAlpha != 255;

    // Copied/adapted from EnTorch2_Draw

    OPEN_DISPS(play->state.gfxCtx);

    gfxP = isTranslucent ? &POLY_OPA_DISP : &POLY_XLU_DISP;
    gfx = *gfxP;

    if (isTranslucent) {
        // SETUPDL_25 but G_RM_AA_ZB_OPA_SURF2 -> G_RM_AA_ZB_XLU_SURF2
        static Gfx setupDL25_xlu[] = {
            gsDPPipeSync(),
            gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
            gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIA_PRIM2),
            gsDPSetOtherMode(G_AD_NOTPATTERN | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE |
                                 G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE,
                             G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_XLU_SURF2),
            gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
            gsSPEndDisplayList(),
        };
        gSPDisplayList(gfx++, setupDL25_xlu);
    } else {
        gfx = Gfx_SetupDL(gfx, SETUPDL_25);
    }

    // env rgb is eyes color, env alpha is whole model alpha
    // note: when drawing non-translucent, env alpha only affects eye alpha
    gDPSetEnvColor(gfx++, this->eyesColor.r, this->eyesColor.g, this->eyesColor.b, this->bodyAlpha);

    gSPSegment(gfx++, 0x0C, isTranslucent ? D_80116280 : gEmptyDL);

    lightDir = (Vec3f){ XYZ(play->envCtx.dirLight1.params.dir) };
    gfx = func_8002E830(&this->actor.world.pos, &play->view.eye, &lightDir, play->state.gfxCtx, gfx, &hilite);

    gfx = SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable, this->skelAnime.dListCount,
                             NULL, muhdl_PostLimbDraw, this, gfx);

    *gfxP = gfx;

    CLOSE_DISPS(play->state.gfxCtx);
}

static void muhdl_draw_smoke(MuhDarkLinkActor* this, PlayState* play) {
    Gfx* restrict* gfxP;
    Gfx* restrict gfx;
    MtxF mf;
    float f;
    static void* dustTextures[] = {
        gDust1Tex, gDust2Tex, gDust3Tex, gDust4Tex, gDust5Tex, gDust6Tex, gDust7Tex, gDust8Tex,
    };

    OPEN_DISPS(play->state.gfxCtx);

    gfxP = &POLY_XLU_DISP;
    gfx = *gfxP;

    f = 1;
    SkinMatrix_SetScale(&mf, f, f, f);
    SkinMatrix_MtxFMtxFMult(&mf, &play->billboardMtxF, &mf);

    gDPPipeSync(gfx++);

    // opt note: merge SETUPDL_0 and the dynamic bits into a static DL
    gfx = Gfx_SetupDL(gfx, SETUPDL_0);
    gDPSetCombineLERP(gfx++,                                       //
                      PRIMITIVE, ENVIRONMENT, TEXEL0, ENVIRONMENT, //
                      PRIMITIVE, 0, TEXEL0, 0,                     //
                      COMBINED, 0, SHADE, 0,                       //
                      0, 0, 0, COMBINED);
    gSPSetGeometryMode(gfx++, G_LIGHTING);

    // inner color
    gDPSetPrimColor(gfx++, 0, 0, 80, 30, 80, 140);
    // outer color
    gDPSetEnvColor(gfx++, 20, 0, 20, 0);

    for (int i = 0; i < ARRAY_COUNT(this->smokeEffects); i++) {
        MuhDarkLinkActorSmokeEffect* eff = &this->smokeEffects[i];

        if (eff->duration != 0) {
            mf.xw = this->bodyPartsPos[eff->limb].x + eff->posOffset.x;
            mf.yw = this->bodyPartsPos[eff->limb].y + eff->posOffset.y;
            mf.zw = this->bodyPartsPos[eff->limb].z + eff->posOffset.z;

            gSPMatrix(gfx++, Matrix_MtxFToNewMtx(&mf, play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

            assert(eff->texIndex >= 0);
            assert(eff->texIndex < ARRAY_COUNT(dustTextures));
            // opt note: repeat texture load
            gSPSegment(gfx++, 0x08, SEGMENTED_TO_VIRTUAL(dustTextures[eff->texIndex]));
            gSPDisplayList(gfx++, gEffDustDL);
        }
    }

    *gfxP = gfx;

    CLOSE_DISPS(play->state.gfxCtx);
}

void MuhDarkLink_Draw(Actor* thisx, PlayState* play) {
    MuhDarkLinkActor* this = (MuhDarkLinkActor*)thisx;

#ifndef NDEBUG
    muhdl_draw_debug(this, play);
#endif

    if (!this->doDraw) {
        return;
    }

    // I don't understand the graphics interaction between translucent dark link and the smoke,
    // but it looks very cool
    muhdl_draw_dark_link(this, play);
    muhdl_draw_smoke(this, play);
}
