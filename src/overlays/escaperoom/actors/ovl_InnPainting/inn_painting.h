#ifndef INN_PAINTING_H
#define INN_PAINTING_H

#include "ultra64.h"
#include "global.h"

typedef struct InnPaintingActorCurseEffect {
    u8 playerBodyPart;
    u8 texIndex; // [0,7] 0=full 7=almost gone
    u8 timer; // [0,duration] ticks up
    u8 duration; // 0 when inactive
    Vec3f posOffset;
} InnPaintingActorCurseEffect;

typedef struct InnPaintingActor {
    Actor actor;
    int bufI;
    int timer;
    s32 objectPohSlot;
    s32 objectInnPaintingSlot;
    SkelAnime poeSkelAnime;
    int poeExitPaintingTimerIni;
    int poeExitPaintingTimer;
    bool drawPoeOutside;
    Vec3f poeOutsidePos;
    Vec3f poeOutsideProjectedPos;
    Vec3s poeOutsideRot;
    int poeCastCurseTimerIni;
    int poeCastCurseTimer;
    InnPaintingActorCurseEffect curseEffects[30];
    int linkTriforceTimerIni;
    int linkTriforceTimer;
} InnPaintingActor;

#endif
