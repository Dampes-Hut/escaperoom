#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "bed_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

CutsceneData LinkSleep[] = {
    CS_BEGIN_CUTSCENE(3, 1000),
    CS_PLAYER_CUE_LIST(1),
        CS_PLAYER_CUE(PLAYER_CUEID_28, 0, 10000, DEG_TO_BINANG(0.000), DEG_TO_BINANG(270.000), DEG_TO_BINANG(0.000), 0, -109, 0, 0, -109, 0, 0.0f, 0.0f, 0.0f),
    CS_CAM_EYE_SPLINE(0, 121),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, 81, 2, 128, 0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, 86, 2, 122, 0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, 91, 2, 116, 0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, 97, 2, 110, 0),
        CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
    CS_CAM_AT_SPLINE(0, 150),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, 77, -2, 124, 0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, 82, -2, 118, 0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, 87, -2, 112, 0),
        CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, 92, -2, 106, 0),
        CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
	CS_END(),
};

