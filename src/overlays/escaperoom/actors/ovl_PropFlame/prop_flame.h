#ifndef PROP_FLAME_H
#define PROP_FLAME_H

#include "ultra64.h"
#include "global.h"

#define PROP_FLAME_SIZE(thisx)   \
    ({                           \
        Actor* _thisx = (thisx); \
        (u16)(_thisx->params) & 0x00FF; \
    })

typedef struct PropFlameActor {
    Actor actor;
} PropFlameActor;

#endif
