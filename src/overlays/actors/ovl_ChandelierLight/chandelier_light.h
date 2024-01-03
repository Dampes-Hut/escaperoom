#ifndef CHANDELIER_LIGHT_H
#define CHANDELIER_LIGHT_H

#include "ultra64.h"
#include "global.h"

#define NUM_CANDLES 6

struct ChandelierCandleInfo {
    Vec3f flamePos; // in world space
    float brightness; // 0-1
};

typedef struct ChandelierLightActor {
    Actor actor;
    LightInfo lightInfo;
    struct LightNode* lightNode;
    int timer;
    struct ChandelierCandleInfo candles[NUM_CANDLES];
    float brightness;
} ChandelierLightActor;

#endif
