#ifndef CHANDELIER_LIGHT_H
#define CHANDELIER_LIGHT_H

#include "ultra64.h"
#include "global.h"

#define NUM_CANDLES 6

typedef struct ChandelierCandleInfo {
    Vec3f flamePos; // in world space
    float brightness; // 0-1
} ChandelierCandleInfo;

typedef struct ChandelierLightActor {
    Actor actor;
    LightInfo lightInfo;
    struct LightNode* lightNode;
    int timer;
    ChandelierCandleInfo candles[NUM_CANDLES];
    float brightness;
} ChandelierLightActor;

#endif
