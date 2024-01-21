#ifndef SWORD_REWARD_H
#define SWORD_REWARD_H

#include "ultra64.h"
#include "global.h"

typedef struct SwordRewardActor {
    Actor actor;
    int csState;
    int csTimer;
    s16 subCamId;
    Vec3f subCamAt, subCamEye;
} SwordRewardActor;

#endif
