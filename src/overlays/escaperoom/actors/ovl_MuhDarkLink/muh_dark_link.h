#ifndef MUH_DARK_LINK_H
#define MUH_DARK_LINK_H

#include "ultra64.h"
#include "global.h"

struct MuhDarkLinkActor;

typedef void (*MuhDarkLinkActorFadeActionFunc)(struct MuhDarkLinkActor*, PlayState*);

typedef struct MuhDarkLinkActorSmokeEffect {
    u8 limb;
    u8 texIndex; // [0,7] 0=full 7=almost gone
    u8 timer; // [0,duration] ticks up
    u8 duration; // 0 when inactive
    Vec3f posOffset;
} MuhDarkLinkActorSmokeEffect;

typedef struct MuhDarkLinkActor {
    Actor actor;
    SkelAnime skelAnime;
    ColliderCylinder colliderCylinder;

    float centerX, centerZ;

    Vec3f bodyPartsPos[PLAYER_LIMB_MAX]; // Note: don't use index 0 (PLAYER_LIMB_NONE)

    Color_RGB8 eyesColor;

    float minSmokeIntensity;
    float maxSmokeIntensity;
    float smokeIntensity; // [0,1]
    int nActiveSmokeEffects;
    MuhDarkLinkActorSmokeEffect smokeEffects[30];

    u8 minBodyAlpha;
    u8 maxBodyAlpha;
    u8 bodyAlpha;

    MuhDarkLinkActorFadeActionFunc fadeActionFunc;
    int fadeTimer;
} MuhDarkLinkActor;

#endif
