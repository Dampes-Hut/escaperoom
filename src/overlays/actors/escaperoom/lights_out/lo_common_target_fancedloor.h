/**
 * header shared between the target and dancefloot LightsOut actors
 */

#ifndef LO_COMMON_H
#define LO_COMMON_H

#include "global.h"

// Maximum grid size is limited by:
// - TMEM size; sqrt(2048) = ~45
// - Texture coordinate precision; TC_OVL_MAX in lights_out_assets.c must fit in s10.5
#define LIGHTSOUT_TARGET_GRID_X 7
#define LIGHTSOUT_TARGET_GRID_Y 3
#define LIGHTSOUT_FANCEDLOOR_GRID_X 8
#define LIGHTSOUT_FANCEDLOOR_GRID_Y 3

#define LIGHTSOUT_TARGET_SCALE_Y 0.004f
#define LIGHTSOUT_FANCEDLOOR_SCALE_Y \
    (1.1f * LIGHTSOUT_TARGET_SCALE_Y * ((float)LIGHTSOUT_FANCEDLOOR_GRID_Y / (float)LIGHTSOUT_TARGET_GRID_Y))

// Maximum 256 states (drawn as CI8)
// However this must be a prime number to do solution checking
// 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109,
// 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
// 241, 251
#define LIGHTSOUT_NUM_STATES 5

typedef struct LightsOutTarget {
    DynaPolyActor dyna;
    u8 overlayAlpha;
    union {
        u8 as_2d[LIGHTSOUT_TARGET_GRID_Y][LIGHTSOUT_TARGET_GRID_X];
        u8 as_1d[LIGHTSOUT_TARGET_GRID_X * LIGHTSOUT_TARGET_GRID_Y];
        u64 force_structure_alignment;
    } grid;
} LightsOutTarget;

#endif
