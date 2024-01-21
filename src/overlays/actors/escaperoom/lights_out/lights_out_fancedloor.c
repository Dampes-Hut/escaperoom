/**
 * Dance floor LightsOut actor, interactive (walk on cells to change state).
 * The goal is to bring a sub-grid of its state to the state of the target LightsOut actor
 * Works in tandem with target LightsOut actor lights_out_target
 */

#include "lights_out_fancedloor.h"

#include "assets/loa_common.inc.c"
#include "assets/loa_sized.inc.c"
#include "lo_common.inc.c"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void LightsOutFanceDloor_Init(Actor* thisx, PlayState* play);
void LightsOutFanceDloor_Destroy(Actor* thisx, PlayState* play);
void LightsOutFanceDloor_Update(Actor* thisx, PlayState* play);
void LightsOutFanceDloor_Draw(Actor* thisx, PlayState* play);

ActorInit Lights_Out_FanceDloor_InitVars = {
    /**/ ACTOR_LIGHTS_OUT_FANCEDLOOR,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(LightsOutFanceDloor),
    /**/ LightsOutFanceDloor_Init,
    /**/ LightsOutFanceDloor_Destroy,
    /**/ LightsOutFanceDloor_Update,
    /**/ LightsOutFanceDloor_Draw,
};

void LightsOutFanceDloor_Init(Actor* thisx, PlayState* play) {
    LightsOutFanceDloor* this = (LightsOutFanceDloor*)thisx;
    CollisionHeader* colHeader = NULL;

    this->dyna.actor.scale = (Vec3f){ LIGHTSOUT_SCALE_X, 0.01f, LIGHTSOUT_SCALE_Y };

    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&gLightsOutCollision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    assert(this->dyna.bgId != BG_ACTOR_MAX);

    // initial state = all 0

#if 0
        for (int y = 0; y < LIGHTSOUT_GRID_Y; y++) {
            for (int x = 0; x < LIGHTSOUT_GRID_X; x++) {
                this->grid[y][x] = 4;
            }
        }
#endif

    this->lastPlayerCellX = 0xFF;
    this->lastPlayerCellZ = 0xFF;
    this->inert = false;
    this->overlayAlpha = 50;
}

void LightsOutFanceDloor_Destroy(Actor* thisx, PlayState* play) {
    LightsOutFanceDloor* this = (LightsOutFanceDloor*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

// (model space cell length) * (actor scale)
#define CELL_SIDE_X ((50000.0f / LIGHTSOUT_GRID_X) * LIGHTSOUT_SCALE_X)
#define CELL_SIDE_Z ((50000.0f / LIGHTSOUT_GRID_Y) * LIGHTSOUT_SCALE_Y)

static s32 LightsOutFanceDloor_GetPlayerCell(LightsOutFanceDloor* this, PlayState* play, u8* x, u8* z) {
    if (!func_800435B4(&this->dyna)) {
        return false;
    }

    Player* player = GET_PLAYER(play);

    f32 cosY = Math_CosS(this->dyna.actor.shape.rot.y);
    f32 sinY = Math_SinS(this->dyna.actor.shape.rot.y);
    f32 deltaX = player->actor.world.pos.x - this->dyna.actor.world.pos.x;
    f32 deltaZ = player->actor.world.pos.z - this->dyna.actor.world.pos.z;

    f32 px = deltaX * cosY - deltaZ * sinY;
    f32 pz = deltaX * sinY + deltaZ * cosY;

    px += CELL_SIDE_X * (f32)LIGHTSOUT_GRID_X / 2;
    pz += CELL_SIDE_Z * (f32)LIGHTSOUT_GRID_Y / 2;

    s32 cellX = (s32)(px / CELL_SIDE_X);
    s32 cellZ = (s32)(pz / CELL_SIDE_Z);

    if (cellX < 0 || cellX > LIGHTSOUT_GRID_X - 1) {
        return false;
    }
    if (cellZ < 0 || cellZ > LIGHTSOUT_GRID_Y - 1) {
        return false;
    }
    *x = cellX;
    *z = cellZ;
    return true;
}

static void LightsOutFanceDloor_SingleCellAdvance(LightsOutFanceDloor* this, u8 x, u8 y) {
    if (LIGHTSOUT_NUM_STATES == 2) {
        this->grid.as_2d[y][x] ^= 1;
    } else {
        this->grid.as_2d[y][x] = (this->grid.as_2d[y][x] + 1) % LIGHTSOUT_NUM_STATES;
    }
}

static void LightsOutFanceDloor_StateAdvance(LightsOutFanceDloor* this, PlayState* play, u8 x, u8 z) {
    // Advance cell state
    LightsOutFanceDloor_SingleCellAdvance(this, x, z);

    // Also update direct neighbors
    if (x > 0) {
        LightsOutFanceDloor_SingleCellAdvance(this, x - 1, z);
    }
    if (x < LIGHTSOUT_GRID_X - 1) {
        LightsOutFanceDloor_SingleCellAdvance(this, x + 1, z);
    }
    if (z > 0) {
        LightsOutFanceDloor_SingleCellAdvance(this, x, z - 1);
    }
    if (z < LIGHTSOUT_GRID_Y - 1) {
        LightsOutFanceDloor_SingleCellAdvance(this, x, z + 1);
    }
}

static bool LightsOutFanceDloor_CheckWinCondition(LightsOutFanceDloor* this, PlayState* play) {
    LightsOutTarget* loTarget = (LightsOutTarget*)Actor_Find(&play->actorCtx, ACTOR_LIGHTS_OUT_TARGET, ACTORCAT_BG);

    assert(loTarget != NULL);
    if (loTarget == NULL)
        return false;

    static_assert(LIGHTSOUT_FANCEDLOOR_GRID_X >= LIGHTSOUT_TARGET_GRID_X &&
                      LIGHTSOUT_FANCEDLOOR_GRID_Y >= LIGHTSOUT_TARGET_GRID_Y,
                  "Dance floor must be at least as big as target");

    // search pattern at coordinates offsetX, offsetY
    for (s32 offsetX = 0; offsetX <= LIGHTSOUT_FANCEDLOOR_GRID_X - LIGHTSOUT_TARGET_GRID_X; offsetX++) {
        for (s32 offsetY = 0; offsetY <= LIGHTSOUT_FANCEDLOOR_GRID_Y - LIGHTSOUT_TARGET_GRID_Y; offsetY++) {

            bool isMatch = true;
            for (s32 targetX = 0; targetX < LIGHTSOUT_TARGET_GRID_X; targetX++) {
                for (s32 targetY = 0; targetY < LIGHTSOUT_TARGET_GRID_Y; targetY++) {
                    s32 danceFloorX = targetX + offsetX;
                    s32 danceFloorY = targetY + offsetY;
                    assert(danceFloorX < LIGHTSOUT_FANCEDLOOR_GRID_X);
                    assert(danceFloorY < LIGHTSOUT_FANCEDLOOR_GRID_Y);

                    if (loTarget->grid.as_2d[targetY][targetX] != this->grid.as_2d[danceFloorY][danceFloorX]) {
                        isMatch = false;
                        goto endPatternMatch;
                    }
                }
            }
        endPatternMatch:
            if (isMatch) {
                return true;
            }
        }
    }

    return false;
}

void LightsOutFanceDloor_Update(Actor* thisx, PlayState* play) {
    LightsOutFanceDloor* this = (LightsOutFanceDloor*)thisx;

    if (this->inert) {
        if (this->overlayAlpha > 30) {
            this->overlayAlpha--;
        }
        return;
    }

    // Update state

    u8 cellX;
    u8 cellZ;
    if (LightsOutFanceDloor_GetPlayerCell(this, play, &cellX, &cellZ)) {
        if (cellX != this->lastPlayerCellX || cellZ != this->lastPlayerCellZ) {
            // Player is in a cell that is different from before
            LightsOutFanceDloor_StateAdvance(this, play, cellX, cellZ);

            this->lastPlayerCellX = cellX;
            this->lastPlayerCellZ = cellZ;

            if (LightsOutFanceDloor_CheckWinCondition(this, play)) {
                // Game is in winning state
                this->inert = true;
                this->overlayAlpha = 100;

                Actor* swordRewardActor = Actor_Find(&play->actorCtx, ACTOR_SWORD_REWARD, ACTORCAT_ITEMACTION);
                assert(swordRewardActor != NULL);
                if (swordRewardActor != NULL) {
                    swordRewardActor->parent = &this->dyna.actor;
                    this->dyna.actor.child = swordRewardActor;
                }
            }
        }
    } else {
        this->lastPlayerCellX = 0xFF;
        this->lastPlayerCellZ = 0xFF;
    }
}

void LightsOutFanceDloor_Draw(Actor* thisx, PlayState* play) {
    LightsOutFanceDloor* this = (LightsOutFanceDloor*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    // Just incase...
    osWritebackDCache(&this->grid, sizeof(this->grid));

    gSPSegment(POLY_OPA_DISP++, 0x08, &this->grid);
    gSPSegment(POLY_OPA_DISP++, 0x09, sLightsOutBoardPalette);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, this->overlayAlpha);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, LIGHTSOUT_SIZED_ASSET_NAME(gLightsOutBoardDL));

    CLOSE_DISPS(play->state.gfxCtx);
}
