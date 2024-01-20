#include "courtyard_tomb_stone.h"

#include "assets/objects/object_haka/object_haka.h"

// initial normal state
#define MOVEDIST_PUSHED 0.0f
// distance moved after pulled
#define MOVEDIST_PULLED -60.0f

#define FLAGS 0

void CourtyardTombStone_Init(Actor* thisx, PlayState* play);
void CourtyardTombStone_Destroy(Actor* thisx, PlayState* play);
void CourtyardTombStone_Update(Actor* thisx, PlayState* play);
void CourtyardTombStone_Draw(Actor* thisx, PlayState* play);

ActorInit CourtyardTombStone_InitVars = {
    /**/ ACTOR_COURTYARD_TOMB_STONE,
    /**/ ACTORCAT_MISC,
    /**/ FLAGS,
    /**/ OBJECT_HAKA,
    /**/ sizeof(CourtyardTombStoneActor),
    /**/ CourtyardTombStone_Init,
    /**/ CourtyardTombStone_Destroy,
    /**/ CourtyardTombStone_Update,
    /**/ CourtyardTombStone_Draw,
};

// ~~stolen~~ copied from z_bg_haka.c func_8087B758
void CourtyardTombStone_UseSandSoundIfOnGraveDirt(CourtyardTombStoneActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Vec3f playerPosRelToThis;

    func_8002DBD0(&this->dyna.actor, &playerPosRelToThis, &player->actor.world.pos);
    if (fabsf(playerPosRelToThis.x) < 34.6f && playerPosRelToThis.z > -112.8f && playerPosRelToThis.z < -36.0f) {
        player->stateFlags2 |= PLAYER_STATE2_FORCE_SAND_FLOOR_SOUND;
    }
}

void CourtyardTombStone_PlayMoveSfx(CourtyardTombStoneActor* this) {
    func_8002F974(&this->dyna.actor, NA_SE_EV_ROCK_SLIDE - SFX_FLAG);
}

void CourtyardTombStone_SetPosition(CourtyardTombStoneActor* this, float offsetForwards) {
    static Vec3f posLocal = { 0 };

    posLocal.z = offsetForwards;

    Matrix_SetTranslateRotateYXZ(XYZ(this->dyna.actor.home.pos), &this->dyna.actor.shape.rot);
    Matrix_MultVec3f(&posLocal, &this->dyna.actor.world.pos);
}

bool CourtyardTombStone_Move(CourtyardTombStoneActor* this, PlayState* play, float target) {
    Player* player = GET_PLAYER(play);

    // pushed:1 middle:0 pulled:-1
    float movedDistNormalized =
        (this->movedDist - (MOVEDIST_PUSHED + MOVEDIST_PULLED) / 2.0f) / ((MOVEDIST_PUSHED - MOVEDIST_PULLED) / 2.0f);
    float strengthFac = (4 + 3 * gSaveContext.numGravesMoved);
    strengthFac = CLAMP_MAX(strengthFac, 20.0f);
    float speed = (1.0f - SQ(movedDistNormalized)) * strengthFac;
    speed = CLAMP(speed, 0.1f, 5.0f);

    if (COURTYARD_TOMB_STONE_INDEX(&this->dyna.actor) == 6) {
        // make this tombstone in particular slower to move, part of the hints
        speed = CLAMP_MAX(speed, 3.0f);
        speed /= 4.0f;
        speed = CLAMP_MIN(speed, 0.1f);
    }

    bool targetReached = Math_StepToF(&this->movedDist, target, speed);
    CourtyardTombStone_SetPosition(this, this->movedDist);

    CourtyardTombStone_PlayMoveSfx(this);

    if (targetReached) {
        gSaveContext.numGravesMoved++;
        this->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~PLAYER_STATE2_GRABBING_PUSHPULL_WALL;
        return true;
    } else {
        return false;
    }
}
void CourtyardTombStone_WaitPull(CourtyardTombStoneActor* this, PlayState* play);
void CourtyardTombStone_PushedMove(CourtyardTombStoneActor* this, PlayState* play) {
    if (CourtyardTombStone_Move(this, play, MOVEDIST_PUSHED)) {
        this->actionFunc = CourtyardTombStone_WaitPull;
        gSaveContext.save.info.courtyard_graves_pulled_flags &= ~(1 << this->index);
    }
}
void CourtyardTombStone_WaitPush(CourtyardTombStoneActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->dyna.unk_150 == 0) {

    } else if (this->dyna.unk_150 > 0.0f) { // push
        this->actionFunc = CourtyardTombStone_PushedMove;
    } else { // pull
        this->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~PLAYER_STATE2_GRABBING_PUSHPULL_WALL;
    }
}
void CourtyardTombStone_PulledMove(CourtyardTombStoneActor* this, PlayState* play) {
    if (CourtyardTombStone_Move(this, play, MOVEDIST_PULLED)) {
        this->actionFunc = CourtyardTombStone_WaitPush;
        gSaveContext.save.info.courtyard_graves_pulled_flags |= (1 << this->index);
    }
}
void CourtyardTombStone_WaitPull(CourtyardTombStoneActor* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->dyna.unk_150 == 0) {

    } else if (this->dyna.unk_150 > 0.0f) { // push
        this->dyna.unk_150 = 0.0f;
        player->stateFlags2 &= ~PLAYER_STATE2_GRABBING_PUSHPULL_WALL;
    } else { // pull
        this->actionFunc = CourtyardTombStone_PulledMove;
    }
}

CourtyardTombStoneActor* driverInstance = NULL;

void CourtyardTombStone_Init(Actor* thisx, PlayState* play) {
    CourtyardTombStoneActor* this = (CourtyardTombStoneActor*)thisx;
    CollisionHeader* colHeader = NULL;

    this->index = COURTYARD_TOMB_STONE_INDEX(&this->dyna.actor);

    Actor_SetScale(&this->dyna.actor, 0.1f);

    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&gGravestoneCol, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);

    if (gSaveContext.save.info.courtyard_graves_pulled_flags & (1 << this->index)) {
        this->actionFunc = CourtyardTombStone_WaitPush;
        this->movedDist = MOVEDIST_PULLED;
    } else {
        this->actionFunc = CourtyardTombStone_WaitPull;
        this->movedDist = MOVEDIST_PUSHED;
    }
    CourtyardTombStone_SetPosition(this, this->movedDist);
}

void CourtyardTombStone_Destroy(Actor* thisx, PlayState* play) {
    CourtyardTombStoneActor* this = (CourtyardTombStoneActor*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);

    if (driverInstance == this) {
        driverInstance = NULL;
    }
}

void CourtyardTombStone_Update(Actor* thisx, PlayState* play) {
    CourtyardTombStoneActor* this = (CourtyardTombStoneActor*)thisx;

    if (driverInstance == NULL || driverInstance->dyna.actor.update == NULL) {
        driverInstance = this;
        this->dyna.actor.flags |= ACTOR_FLAG_NOCULL_UPDATE;
    }

    CourtyardTombStone_UseSandSoundIfOnGraveDirt(this, play);

    this->actionFunc(this, play);

    if (driverInstance == this) {
        if (gSaveContext.save.info.courtyard_graves_pulled_flags == CORRECT_COURTYARD_GRAVES_PULLED_FLAGS) {
            rmonPrintf("correct tombstones!\n");
            // TODO
            Sfx_PlaySfxCentered(NA_SE_SY_CORRECT_CHIME);
        }
    }
}

void CourtyardTombStone_Draw(Actor* thisx, PlayState* play) {
    CourtyardTombStoneActor* this = (CourtyardTombStoneActor*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_DrawDListOpa(play, gGravestoneStoneDL);

    CLOSE_DISPS(play->state.gfxCtx);
}
