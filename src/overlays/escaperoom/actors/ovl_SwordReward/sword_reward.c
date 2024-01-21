#include "sword_reward.h"

#include "assets/objects/object_toki_objects/object_toki_objects.h"

#define FLAGS 0

void SwordReward_Init(Actor* thisx, PlayState* play);
void SwordReward_Destroy(Actor* thisx, PlayState* play);
void SwordReward_Update(Actor* thisx, PlayState* play);
void SwordReward_Draw(Actor* thisx, PlayState* play);

ActorInit SwordReward_InitVars = {
    /**/ ACTOR_SWORD_REWARD,
    /**/ ACTORCAT_ITEMACTION,
    /**/ FLAGS,
    /**/ OBJECT_TOKI_OBJECTS,
    /**/ sizeof(SwordRewardActor),
    /**/ SwordReward_Init,
    /**/ SwordReward_Destroy,
    /**/ SwordReward_Update,
    /**/ NULL,
};

void SwordReward_UpdateNoop(Actor* thisx, PlayState* play) {
    // no-op
}

void SwordReward_Init(Actor* thisx, PlayState* play) {
    SwordRewardActor* this = (SwordRewardActor*)thisx;

    Actor_SetScale(&this->actor, 0.04f);
    this->actor.world.pos.y = this->actor.home.pos.y - 80.0f;

    if (Flags_GetCollectible(play, this->actor.params)) {
        this->actor.update = SwordReward_UpdateNoop;
    }
}

void SwordReward_Destroy(Actor* thisx, PlayState* play) {
    SwordRewardActor* this = (SwordRewardActor*)thisx;
}

void SwordReward_Update(Actor* thisx, PlayState* play) {
    SwordRewardActor* this = (SwordRewardActor*)thisx;

    if (Actor_HasParent(&this->actor, play)) {
        Player* player = GET_PLAYER(play);

        rmonPrintf("SwordReward_Update: sword reward ON\n");
        rmonPrintf("csState=%d csTimer=%d\n", this->csState, this->csTimer);

        if (this->csState == 0) {
            this->csState++;

            this->actor.draw = SwordReward_Draw;

            Cutscene_StartManual(play, &play->csCtx);
            this->subCamId = Play_CreateSubCamera(play);
            Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
            Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);

            this->subCamAt = this->actor.home.pos;
            this->subCamAt.y += 30.0f;
            Math_Vec3f_Sum(&this->subCamAt, &(Vec3f){ -35.0f, 20.0f, 80.0f }, &this->subCamEye);
            Play_SetCameraAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);
            this->csTimer = 0;
        }

        bool stillMovingUp = Math_StepToF(&this->actor.world.pos.y, this->actor.home.pos.y + 20.0f, 2.0f);

        if (this->csState == 1 && stillMovingUp) {
            if (this->csTimer == 5) {
                Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
            }
            this->csTimer++;
            if (this->csTimer >= 30) {
                this->csState++;

                Cutscene_StopManual(play, &play->csCtx);

                assert(this->subCamId != SUB_CAM_ID_DONE);
                if (this->subCamId != SUB_CAM_ID_DONE) {
                    Play_ReturnToMainCam(play, this->subCamId, 0);
                    this->subCamId = SUB_CAM_ID_DONE;
                }
            }
        }

        if (!stillMovingUp) {
            float f = 3.0f;
            Vec3f vel = { Rand_CenteredFloat(f), Rand_ZeroFloat(f), Rand_CenteredFloat(f) };
            Vec3f accel = vel;
            Math_Vec3f_Scale(&accel, -0.2f);
            func_8002865C(play, &this->actor.home.pos, &vel, &accel, 300, 20);
        }

        if (this->csState == 2 && this->actor.xzDistToPlayer < 20.0f) {
            rmonPrintf("SwordReward_Update: player yoink\n");

            this->actor.draw = NULL;

            gSaveContext.save.info.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
            Inventory_ChangeEquipment(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_MASTER);
            gSaveContext.save.info.equips.buttonItems[0] = ITEM_SWORD_MASTER;
            Interface_LoadItemIcon1(play, 0);

            // ???
            // player->itemAction = PLAYER_IA_SWORD_MASTER;
            player->heldItemAction = PLAYER_IA_SWORD_MASTER;
            // player->heldItemId = ITEM_SWORD_MASTER;
            // player->currentSwordItemId = ITEM_SWORD_MASTER;
            // player->leftHandType = PLAYER_MODELTYPE_LH_SWORD;
            Player_SetEquipmentData(play, player);

            Flags_SetCollectible(play, this->actor.params);

            this->csState++;

            Cutscene_StartManual(play, &play->csCtx);
            this->subCamId = Play_CreateSubCamera(play);
            Play_ChangeCameraStatus(play, CAM_ID_MAIN, CAM_STAT_WAIT);
            Play_ChangeCameraStatus(play, this->subCamId, CAM_STAT_ACTIVE);

            // swing sword
            Player_SetCsAction(play, NULL, PLAYER_CSACTION_90);

            this->subCamAt = player->actor.world.pos;
            this->subCamAt.y += 30.0f;
            Math_Vec3f_Sum(&this->subCamAt, &(Vec3f){ -35.0f, 20.0f, 80.0f }, &this->subCamEye);
            Play_SetCameraAtEye(play, this->subCamId, &this->subCamAt, &this->subCamEye);
            this->csTimer = 0;
        }

        if (this->csState == 3) {
            this->csTimer++;
            if (this->csTimer >= 72) {
                player->heldItemAction = PLAYER_IA_NONE;
                Player_SetEquipmentData(play, player);
                Player_SetCsAction(play, NULL, PLAYER_CSACTION_49);

                this->csState++;

                Cutscene_StopManual(play, &play->csCtx);

                assert(this->subCamId != SUB_CAM_ID_DONE);
                if (this->subCamId != SUB_CAM_ID_DONE) {
                    Play_ReturnToMainCam(play, this->subCamId, 0);
                    this->subCamId = SUB_CAM_ID_DONE;
                }
            }
        }

        if (this->csState == 4) {
            rmonPrintf("SwordReward_Update: Actor_Kill\n");
            Actor_Kill(&this->actor);
        }
    }
}

void SwordReward_Draw(Actor* thisx, PlayState* play) {
    SwordRewardActor* this = (SwordRewardActor*)thisx;

    // much copied from BgTokiSwd_Draw

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    func_8002EBCC(&this->actor, play, 0);

    gSPSegment(POLY_OPA_DISP++, 0x08, Gfx_TexScroll(play->state.gfxCtx, 0, -(play->gameplayFrames % 0x80), 32, 32));
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_toki_objects_DL_001BD0);

    CLOSE_DISPS(play->state.gfxCtx);
}
