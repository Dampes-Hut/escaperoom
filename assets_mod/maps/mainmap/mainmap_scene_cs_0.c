#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "mainmap_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

CutsceneData gIntroPart2Cs[] = {
    CS_BEGIN_CUTSCENE(5, 587),
        CS_DESTINATION(CS_DEST_MAINMAP_COFFIN, 245, 0),
        CS_TEXT_LIST(1),
            CS_TEXT(0x600, 40, 200, CS_TEXT_NORMAL, 0x0000, 0x0000),
        CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_8_0, 2),
            CS_ACTOR_CUE(0x0000, 0, 123, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -233, -68, -286, -276, -68, -177, 0.0f, 0.0f, 0.0f),
            CS_ACTOR_CUE(0x0000, 123, 239, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -276, -68, -177, -160, -69, -204, 0.0f, 0.0f, 0.0f),
        CS_CAM_EYE_SPLINE(0, 121),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -226, -40, 0, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -226, -40, 0, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -226, -40, 0, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -226, -40, 0, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_AT_SPLINE(0, 150),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -226, -40, -60, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -226, -40, -60, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -226, -40, -60, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -226, -40, -60, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
    CS_END(),
};

