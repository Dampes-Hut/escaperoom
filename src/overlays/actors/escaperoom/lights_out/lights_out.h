#ifndef LIGHTS_OUT_H
#define LIGHTS_OUT_H

#include "global.h"

struct LightsOut;

typedef void (*LightsOutActionFunc)(struct LightsOut*, PlayState*);

// Maximum grid size is limited by:
// - TMEM size; sqrt(2048) = ~45
// - Texture coordinate precision; TC_OVL_MAX in lights_out_assets.c must fit in s10.5
#define LIGHTSOUT_GRID_X 3
#define LIGHTSOUT_GRID_Y 2
#define LIGHTSOUT_GRID_XY (LIGHTSOUT_GRID_Y * LIGHTSOUT_GRID_X)

// Maximum 256 states (drawn as CI8)
// However this must be a prime number to do solution checking
// 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109,
// 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
// 241, 251
#define LIGHTSOUT_NUM_STATES 3

typedef struct LightsOut {
    DynaPolyActor dyna;
    u8 overlayAlpha;
    u8 inert;
    u8 lastPlayerCellX;
    u8 lastPlayerCellZ;
    union {
        u8 grid[LIGHTSOUT_GRID_Y][LIGHTSOUT_GRID_X];
        u8 gridArray[LIGHTSOUT_GRID_XY];
        u64 force_structure_alignment;
    };
} LightsOut;

#endif
