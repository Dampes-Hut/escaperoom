#ifndef LIGHTS_OUT_H
#define LIGHTS_OUT_H

#include "global.h"

#include "lo_common_target_fancedloor.h"

#define LIGHTSOUT_SIZED_ASSET_NAME(baseSymName) baseSymName ## _target

#define LIGHTSOUT_GRID_X LIGHTSOUT_TARGET_GRID_X
#define LIGHTSOUT_GRID_Y LIGHTSOUT_TARGET_GRID_Y
#define LIGHTSOUT_GRID_XY (LIGHTSOUT_GRID_Y * LIGHTSOUT_GRID_X)

#define LIGHTSOUT_SCALE_Y LIGHTSOUT_TARGET_SCALE_Y
#define LIGHTSOUT_SCALE_X (LIGHTSOUT_SCALE_Y * (float)LIGHTSOUT_GRID_X / (float)LIGHTSOUT_GRID_Y)

#endif
