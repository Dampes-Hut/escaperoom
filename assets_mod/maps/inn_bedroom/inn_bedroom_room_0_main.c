#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "inn_bedroom_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

/**
 * Header Child Day (Default)
*/
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER00_OBJECTLIST 2
#define LENGTH_INN_BEDROOM_ROOM_0_HEADER00_ACTORLIST 1
SceneCmd inn_bedroom_room_0_header00[] = {
    SCENE_CMD_ECHO_SETTINGS(0x00),
    SCENE_CMD_ROOM_BEHAVIOR(0x00, 0x00, false, false),
    SCENE_CMD_SKYBOX_DISABLES(false, false),
    SCENE_CMD_TIME_SETTINGS(0xFF, 0xFF, 10),
    SCENE_CMD_ROOM_SHAPE(&inn_bedroom_room_0_shapeHeader),
    SCENE_CMD_OBJECT_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER00_OBJECTLIST, inn_bedroom_room_0_header00_objectList),
    SCENE_CMD_ACTOR_LIST(LENGTH_INN_BEDROOM_ROOM_0_HEADER00_ACTORLIST, inn_bedroom_room_0_header00_actorList),
    SCENE_CMD_END(),
};

s16 inn_bedroom_room_0_header00_objectList[LENGTH_INN_BEDROOM_ROOM_0_HEADER00_OBJECTLIST] = {
    OBJECT_POH,
    OBJECT_INN_PAINTING,
};

ActorEntry inn_bedroom_room_0_header00_actorList[LENGTH_INN_BEDROOM_ROOM_0_HEADER00_ACTORLIST] = {
    // Custom Actor
    {
        /* Actor ID   */ ACTOR_INN_PAINTING,
        /* Position   */ { -157, -53, -27 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(180.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0000
    },
};

