#include "tower_top_poe_manager.h"

#include "../../../actors/ovl_En_Poh/z_en_poh.h"

#define FLAGS 0

void TowerTopPoeManager_Init(Actor* thisx, PlayState* play);
void TowerTopPoeManager_Destroy(Actor* thisx, PlayState* play);
void TowerTopPoeManager_Update(Actor* thisx, PlayState* play);
void TowerTopPoeManager_Draw(Actor* thisx, PlayState* play);

ActorInit TowerTopPoeManager_InitVars = {
    /**/ ACTOR_TOWER_TOP_POE_MANAGER,
    /**/ ACTORCAT_MISC,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(TowerTopPoeManagerActor),
    /**/ TowerTopPoeManager_Init,
    /**/ TowerTopPoeManager_Destroy,
    /**/ TowerTopPoeManager_Update,
    /**/ TowerTopPoeManager_Draw,
};

void TowerTopPoeManager_Init(Actor* thisx, PlayState* play) {
    TowerTopPoeManagerActor* this = (TowerTopPoeManagerActor*)thisx;
    Player* player = GET_PLAYER(play);

    if (player->actor.world.pos.y < 1000.0f) {
        // player isn't at the tower top
        Actor_Kill(&this->actor);
        return;
    }

#ifndef NDEBUG
#if 1
    gSaveContext.save.info.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_MASTER);
    Inventory_ChangeEquipment(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_MASTER);
    gSaveContext.save.info.equips.buttonItems[0] = ITEM_SWORD_MASTER;
    Interface_LoadItemIcon1(play, 0);
#endif
#endif

    this->state = TTPM_STATE_INIT;
}

void TowerTopPoeManager_Destroy(Actor* thisx, PlayState* play) {
    TowerTopPoeManagerActor* this = (TowerTopPoeManagerActor*)thisx;
}

static void ttpm_setup_credits(TowerTopPoeManagerActor* this, PlayState* play) {
    this->state = TTPM_STATE_CREDITS;
    this->timer = 0;
}

void TowerTopPoeManager_Update(Actor* thisx, PlayState* play) {
    TowerTopPoeManagerActor* this = (TowerTopPoeManagerActor*)thisx;

    switch (this->state) {
        case TTPM_STATE_INIT: {
            Actor* poe = Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_POH,
                                            XYZ(this->actor.world.pos), XYZ(this->actor.shape.rot), EN_POH_NORMAL);
            assert(poe != NULL);
            if (poe != NULL) {
                this->state = TTPM_STATE_WAIT_POE_DEFEATED;
            } else {
                // funny fail-safe I guess
                ttpm_setup_credits(this, play);
            }
        } break;

        case TTPM_STATE_WAIT_POE_DEFEATED: {
            if (this->actor.child == NULL || this->actor.child->update == NULL) {
                // poe Actor_Kill'd, has been defeated
                ttpm_setup_credits(this, play);
            }
        } break;

        case TTPM_STATE_CREDITS: {
            if (this->timer < 100) {
                this->timer++;
                if (this->timer == 40) {
                    Message_StartTextbox(play, 0x700, NULL);
                }
            }
            if (this->timer >= 100 && Message_GetState(&play->msgCtx) == TEXT_STATE_NONE) {
                this->timer++;
                if (this->timer > 160) {
                    play->nextEntranceIndex = ENTR_INN_BEDROOM_0;
                    gSaveContext.nextCutsceneIndex = 0xFFF0;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                }
            }
        } break;
    }
}

void TowerTopPoeManager_Draw(Actor* thisx, PlayState* play) {
    TowerTopPoeManagerActor* this = (TowerTopPoeManagerActor*)thisx;
}
