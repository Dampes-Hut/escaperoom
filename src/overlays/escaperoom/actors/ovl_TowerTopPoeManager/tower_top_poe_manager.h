#ifndef TOWER_TOP_POE_MANAGER_H
#define TOWER_TOP_POE_MANAGER_H

#include "ultra64.h"
#include "global.h"

typedef struct TowerTopPoeManagerActor {
    Actor actor;
    enum {
        TTPM_STATE_INIT,
        TTPM_STATE_WAIT_POE_DEFEATED,
        TTPM_STATE_CREDITS
    } state;
    int timer;
} TowerTopPoeManagerActor;

#endif
