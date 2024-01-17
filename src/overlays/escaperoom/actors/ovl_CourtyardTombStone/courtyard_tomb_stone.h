#ifndef COURTYARD_TOMB_STONE_H
#define COURTYARD_TOMB_STONE_H

#include "ultra64.h"
#include "global.h"

#define COURTYARD_TOMB_STONE_INDEX(thisx) \
    ({                                    \
        Actor* _thisx = (thisx);          \
        _thisx->params;                   \
    })

struct CourtyardTombStoneActor;
typedef void (*CourtyardTombStoneActorActionFunc)(struct CourtyardTombStoneActor*, PlayState*);

typedef struct CourtyardTombStoneActor {
    DynaPolyActor dyna;
    CourtyardTombStoneActorActionFunc actionFunc;
    float movedDist; // measured along actor's forward +Z
    int index;
} CourtyardTombStoneActor;

#endif
