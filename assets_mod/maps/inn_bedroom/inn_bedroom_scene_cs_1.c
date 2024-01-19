#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "inn_bedroom_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

CutsceneData gInnBedroomIntroCs[] = {
    CS_BEGIN_CUTSCENE(7, 10000),
        CS_TRANSITION(CS_TRANS_BLACK_FILL, 0, 650),
        CS_TEXT_LIST(6),
            CS_TEXT(2, 0, 10, CS_TEXT_NORMAL, 0x0000, 0x0000),
            CS_TEXT(3, 180, 190, CS_TEXT_NORMAL, 0x0000, 0x0000),
            CS_TEXT(4, 195, 220, CS_TEXT_NORMAL, 0x0000, 0x0000),
            CS_TEXT(5, 230, 250, CS_TEXT_NORMAL, 0x0000, 0x0000),
            CS_TEXT(6, 380, 390, CS_TEXT_NORMAL, 0x0000, 0x0000),
            CS_TEXT(7, 455, 465, CS_TEXT_NORMAL, 0x0000, 0x0000),
        CS_MISC_LIST(1),
            CS_MISC(CS_MISC_SFX_LINK_ADVENTURING, 2, 10000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
        CS_TRANSITION(CS_TRANS_BLACK_FILL_OUT, 651, 700),
        CS_PLAYER_CUE_LIST(4),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 0, 648, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -171, -97, -190, -170, -97, -190, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 648, 880, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -170, -97, -190, -127, -97, 41, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_14, 880, 930, DEG_TO_BINANG(0.000), DEG_TO_BINANG(15.051), DEG_TO_BINANG(0.000), -127, -97, 41, -128, -97, 43, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_49, 930, 1000, DEG_TO_BINANG(0.000), DEG_TO_BINANG(13.123), DEG_TO_BINANG(0.000), -128, -97, 43, 0, 0, 0, 0.0f, 0.0f, 0.0f),
        CS_CAM_EYE_SPLINE(0, 1823),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -30, 0, -31, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -41, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -7, 0, -59, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -3, 0, -75, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -3, 0, -75, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -3, 0, -75, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -152, 0, -53, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -152, 0, -53, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -215, 20, 73, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_AT_SPLINE(0, 1852),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -233, -36, -167, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 700, 60.0f, -233, -36, -167, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 700, 60.0f, -233, -36, -167, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 64, 60.0f, -233, -36, -167, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 73, 60.0f, -227, -36, -68, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 73, 60.0f, -120, -36, 68, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 80, 60.0f, -120, -36, 68, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 72, 60.0f, -36, -59, 133, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -36, -59, 133, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
    CS_END(),
};

