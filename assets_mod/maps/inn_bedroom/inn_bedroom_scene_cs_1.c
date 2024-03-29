#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "inn_bedroom_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

CutsceneData gInnBedroomIntroCs[] = {
    CS_BEGIN_CUTSCENE(14, 10000),
        CS_DESTINATION(CS_DEST_INTROCS_PART2, 1447, 0),
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
        CS_TRANSITION(CS_TRANS_BLACK_FILL_OUT, 651, 674),
        CS_TRANSITION(CS_TRANS_BLACK_FILL_IN, 980, 1090),
        CS_TRANSITION(CS_TRANS_BLACK_FILL_OUT, 1100, 1150),
        CS_ACTOR_CUE_LIST(CS_CMD_ACTOR_CUE_INN_PAINTING, 6),
            CS_ACTOR_CUE(INNPAINTING_CUEID_POE_UNFREEZE, 1180, 1240, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f),
            CS_ACTOR_CUE(INNPAINTING_CUEID_POE_EXITPAINTING, 1240, 1320, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f),
            CS_ACTOR_CUE(INNPAINTING_CUEID_POE_CASTCURSE, 1320, 1360, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f),
            CS_ACTOR_CUE(INNPAINTING_CUEID_LINK_TRIFORCE, 1360, 1390, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f),
            CS_ACTOR_CUE(INNPAINTING_CUEID_POE_SURPRISED, 1390, 1400, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f),
            CS_ACTOR_CUE(INNPAINTING_CUEID_NOOP, 1400, 1401, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), 0, 0, 0, 0, 0, 0, 0.0f, 0.0f, 0.0f),
        CS_PLAYER_CUE_LIST(11),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 0, 650, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -171, -97, -190, -170, -97, -190, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_61, 650, 731, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -170, -97, -190, -171, -97, -189, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 731, 801, DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), -171, -97, -189, -127, -97, 44, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_49, 801, 840, DEG_TO_BINANG(0.000), DEG_TO_BINANG(13.123), DEG_TO_BINANG(0.000), -127, -97, 44, -127, -97, 44, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 840, 847, DEG_TO_BINANG(0.000), DEG_TO_BINANG(16.238), DEG_TO_BINANG(0.000), -127, -97, 44, -95, -97, 50, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 847, 855, DEG_TO_BINANG(0.000), DEG_TO_BINANG(157.885), DEG_TO_BINANG(0.000), -95, -97, 50, -70, -97, 67, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 855, 868, DEG_TO_BINANG(0.000), DEG_TO_BINANG(130.946), DEG_TO_BINANG(0.000), -70, -97, 67, -53, -97, 106, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 868, 880, DEG_TO_BINANG(0.000), DEG_TO_BINANG(115.576), DEG_TO_BINANG(0.000), -53, -97, 106, -42, -97, 146, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 880, 894, DEG_TO_BINANG(0.000), DEG_TO_BINANG(17.781), DEG_TO_BINANG(0.000), -42, -97, 146, -28, -97, 162, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_1, 894, 901, DEG_TO_BINANG(0.000), DEG_TO_BINANG(78.113), DEG_TO_BINANG(0.000), -28, -97, 162, -27, -97, 162, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_14, 901, 1090, DEG_TO_BINANG(0.000), DEG_TO_BINANG(90.786), DEG_TO_BINANG(0.000), -27, -97, 162, 20, -88, 166, 0.0f, 0.0f, 0.0f),
        CS_PLAYER_CUE_LIST(3),
            CS_PLAYER_CUE(PLAYER_CUEID_SLEEPING, 1099, 1330, DEG_TO_BINANG(0.000), DEG_TO_BINANG(90.000), DEG_TO_BINANG(0.000), 18, -81, 168, 18, -81, 168, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_29, 1330, 1350, DEG_TO_BINANG(0.000), DEG_TO_BINANG(90.000), DEG_TO_BINANG(0.000), 18, -81, 168, 18, -81, 168, 0.0f, 0.0f, 0.0f),
            CS_PLAYER_CUE(PLAYER_CUEID_SLEEPING, 1350, 1502, DEG_TO_BINANG(0.000), DEG_TO_BINANG(90.000), DEG_TO_BINANG(0.000), 18, -81, 168, 0, 0, 0, 0.0f, 0.0f, 0.0f),
        CS_CAM_EYE_SPLINE(649, 1020),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -18, 0, -42, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_AT_SPLINE(649, 1049),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -71, -10, -68, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 98, 60.0f, -71, -10, -68, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 58, 60.0f, -71, -10, -68, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 64, 60.0f, -76, -15, -48, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -56, -15, 2, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -56, -15, 2, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -32, -9, 16, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -32, -9, 16, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_EYE_SPLINE(1099, 1620),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -202, 20, 128, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -202, 20, 128, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -202, 20, 128, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -137, -2, 73, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -137, -2, 73, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -86, 43, 69, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 0, 60.0f, -86, 43, 69, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
        CS_CAM_AT_SPLINE(1099, 1649),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -148, 7, 150, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 67, 60.0f, -148, 7, 150, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 73, 60.0f, -148, 7, 150, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 116, 60.0f, -91, -11, 110, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 42, 60.0f, -91, -11, 110, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 162, 60.0f, -62, 9, 118, 0),
            CS_CAM_POINT(CS_CAM_CONTINUE, 0, 30, 60.0f, -62, 9, 118, 0),
            CS_CAM_POINT(CS_CAM_STOP, 0, 0, 0.0f, 0, 0, 0, 0),
    CS_END(),
};

