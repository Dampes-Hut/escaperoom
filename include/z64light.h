#ifndef Z64LIGHT_H
#define Z64LIGHT_H

#include "ultra64.h"
#include "ultra64/gbi.h"
#include "z64math.h"
#include "color.h"

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 z;
    /* 0x6 */ u8 color[3];
    /* 0x9 */ u8 drawGlow;
    /* 0xA */ s16 radius;
} LightPoint; // size = 0xC

typedef struct {
    /* 0x0 */ s8 x;
    /* 0x1 */ s8 y;
    /* 0x2 */ s8 z;
    /* 0x3 */ u8 color[3];
} LightDirectional; // size = 0x6

typedef union {
    LightPoint point;
    LightDirectional dir;
} LightParams; // size = 0xC

typedef struct {
    /* 0x0 */ u8 type;
    /* 0x2 */ LightParams params;
} LightInfo; // size = 0xE

#if 0
typedef struct Lights {
    /* 0x00 */ u8 numLights;
    /* 0x08 */ Lightsn l;
} Lights; // size = 0x80
#endif

// Maximum number of lights supported by the microcode.
#define MAX_LIGHTS 7

typedef struct Lights {
    u8 numLights;
    f32 distances[MAX_LIGHTS];
    Lightsn l;
} Lights;

typedef struct LightNode {
    /* 0x0 */ LightInfo* info;
    /* 0x4 */ struct LightNode* prev;
    /* 0x8 */ struct LightNode* next;
#ifndef NDEBUG
    char file[256]; // copy the filename since a pointer may become dangling (if the string is in a z64 overlay)
    int line;
    char additional_context[256];
#endif
} LightNode;

#define ENV_FOGNEAR_MAX 996
#define ENV_ZFAR_MAX 12800

typedef struct {
    /* 0x0 */ LightNode* listHead;
    /* 0x4 */ u8 ambientColor[3];
    /* 0x7 */ u8 fogColor[3];
    /* 0xA */ s16 fogNear; // how close until fog starts taking effect. range 0 - ENV_FOGNEAR_MAX
    /* 0xC */ s16 zFar; // draw distance. range 0 - ENV_ZFAR_MAX
} LightContext; // size = 0x10

typedef enum {
    /* 0x00 */ LIGHT_POINT_NOGLOW,
    /* 0x01 */ LIGHT_DIRECTIONAL,
    /* 0x02 */ LIGHT_POINT_GLOW,
    /* 0x03 */ LIGHT_TYPE_MAX
} LightType;

#if 0
typedef void (*LightsBindFunc)(Lights* lights, LightParams* params, Vec3f* vec);
#endif

struct PlayState;
typedef void (*LightsBindFunc)(Lights* lights, LightParams* params, Vec3f* objPos, struct PlayState* play);

#endif
