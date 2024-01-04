#ifndef INN_PAINTING_H
#define INN_PAINTING_H

#include "ultra64.h"
#include "global.h"

typedef struct InnPaintingActor {
    Actor actor;
    int bufI;
    int timer;
    s32 objectPohSlot;
    s32 objectInnPaintingSlot;
    SkelAnime poeSkelAnime;
} InnPaintingActor;

#endif
