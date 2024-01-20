#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "inn_bedroom_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

CutsceneData TitleScreenCutscene[] = {
    CS_BEGIN_CUTSCENE(6, 2047),
        CS_DESTINATION(CS_DEST_TITLE_SCREEN_LOOP, 1700, 0),
        CS_PLAYER_CUE_LIST(1),
            CS_PLAYER_CUE(PLAYER_CUEID_SLEEPING, 0, 1, DEG_TO_BINANG(0.000), DEG_TO_BINANG(90.000), DEG_TO_BINANG(0.000), 18, -81, 168, 0, 0, 0, 0.0f, 0.0f, 0.0f),
        CS_CAM_EYE_SPLINE(0, 848),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -19, 0, 4, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_AT_SPLINE(0, 871),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -122, -64, 104, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 103, 60.0f, -122, -64, 104, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 172, 60.0f, -122, -64, 104, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 118, 60.0f, -231, -24, -71, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 93, 60.0f, -233, -21, -186, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 307, 60.0f, -172, -40, -210, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 24, 60.0f, -140, -44, -210, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_EYE_SPLINE(500, 2046),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -179, 0, 140, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_AT_SPLINE(500, 2075),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 19, 60.0f, -2, -44, -57, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 166, 60.0f, 39, -40, 3, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 144, 60.0f, 59, -68, 88, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 107, 60.0f, 59, -71, 127, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 114, 60.0f, 59, -68, 184, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 111, 60.0f, 44, -62, 236, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 249, 60.0f, 59, -62, 225, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 129, 60.0f, 59, -57, 159, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 227, 60.0f, 59, 1, 87, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 249, 60.0f, -35, 94, 64, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -35, 94, 64, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
    CS_END(),
};

