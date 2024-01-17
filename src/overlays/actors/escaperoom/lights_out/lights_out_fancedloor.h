#ifndef LIGHTS_OUT_H
#define LIGHTS_OUT_H

#include "global.h"

#include "lo_common_target_fancedloor.h"

#define LIGHTSOUT_SIZED_ASSET_NAME(baseSymName) baseSymName ## _fancedloor

#define LIGHTSOUT_GRID_X LIGHTSOUT_FANCEDLOOR_GRID_X
#define LIGHTSOUT_GRID_Y LIGHTSOUT_FANCEDLOOR_GRID_Y
#define LIGHTSOUT_GRID_XY (LIGHTSOUT_GRID_Y * LIGHTSOUT_GRID_X)

#define LIGHTSOUT_SCALE_Y LIGHTSOUT_FANCEDLOOR_SCALE_Y
#define LIGHTSOUT_SCALE_X (LIGHTSOUT_SCALE_Y * (float)LIGHTSOUT_GRID_X / (float)LIGHTSOUT_GRID_Y)

typedef struct LightsOutFanceDloor {
    DynaPolyActor dyna;
    u8 overlayAlpha;
    u8 inert;
    u8 lastPlayerCellX;
    u8 lastPlayerCellZ;
    union {
        u8 as_2d[LIGHTSOUT_GRID_Y][LIGHTSOUT_GRID_X];
        u8 as_1d[LIGHTSOUT_GRID_XY];
        u64 force_structure_alignment;
    } grid;
} LightsOutFanceDloor;

#endif
