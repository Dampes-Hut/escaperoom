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
SceneCmd inn_bedroom_scene_header00[] = {
    SCENE_CMD_ALTERNATE_HEADER_LIST(inn_bedroom_scene_alternateHeaders),
    SCENE_CMD_SOUND_SETTINGS(0x00, 0x13, NA_BGM_LINK_HOUSE),
    SCENE_CMD_ROOM_LIST(1, inn_bedroom_scene_roomList),
    SCENE_CMD_MISC_SETTINGS(0x00, 0x00),
    SCENE_CMD_COL_HEADER(&inn_bedroom_collisionHeader),
    SCENE_CMD_SPECIAL_FILES(0x00, OBJECT_GAMEPLAY_FIELD_KEEP),
    SCENE_CMD_SKYBOX_SETTINGS(0x05, 0x00, LIGHT_MODE_SETTINGS),
    SCENE_CMD_ENTRANCE_LIST(inn_bedroom_scene_header00_entranceList),
    SCENE_CMD_SPAWN_LIST(1, inn_bedroom_scene_header00_playerEntryList),
    SCENE_CMD_ENV_LIGHT_SETTINGS(1, inn_bedroom_scene_header00_lightSettings),
    SCENE_CMD_TRANSITION_ACTOR_LIST(1, inn_bedroom_scene_header00_transitionActors),
    SCENE_CMD_END(),
};

SceneCmd* inn_bedroom_scene_alternateHeaders[] = {
    NULL,
    NULL,
    NULL,
    inn_bedroom_scene_header04,
};

RomFile inn_bedroom_scene_roomList[] = {
    { (u32)_inn_bedroom_room_0SegmentRomStart, (u32)_inn_bedroom_room_0SegmentRomEnd },
};

ActorEntry inn_bedroom_scene_header00_playerEntryList[] = {
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { -170, -97, -177 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
};

TransitionActorEntry inn_bedroom_scene_header00_transitionActors[] = {
    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 0, 0x00, 0, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { -172, -97, -209 },
        /* Rotation Y                     */ DEG_TO_BINANG(0.000),
        /* Parameters                     */ 0x0000
    },
};

Spawn inn_bedroom_scene_header00_entranceList[] = {
    // { Spawn Actor List Index, Room Index }
    { 0, 0 },
};

EnvLightSettings inn_bedroom_scene_header00_lightSettings[1] = {
    // Indoor No. 1 Lighting
    {
        {    32,    41,    50 },   // Ambient Color
        {    73,    73,    73 },   // Diffuse0 Direction
        {     0,     0,     0 },   // Diffuse0 Color
        {   -73,   -73,   -73 },   // Diffuse1 Direction
        {     0,     0,     0 },   // Diffuse1 Color
        {   196,   182,   176 },   // Fog Color
        ((1 << 10) | 993),         // Blend Rate & Fog Near
        12800,                     // Fog Far
    },
};

/**
 * Header Cutscene No. 1
*/
SceneCmd inn_bedroom_scene_header04[] = {
    SCENE_CMD_SOUND_SETTINGS(0x00, 0x13, NA_BGM_LINK_HOUSE),
    SCENE_CMD_ROOM_LIST(1, inn_bedroom_scene_roomList),
    SCENE_CMD_MISC_SETTINGS(0x00, 0x00),
    SCENE_CMD_COL_HEADER(&inn_bedroom_collisionHeader),
    SCENE_CMD_SPECIAL_FILES(0x00, OBJECT_GAMEPLAY_FIELD_KEEP),
    SCENE_CMD_SKYBOX_SETTINGS(0x05, 0x00, LIGHT_MODE_SETTINGS),
    SCENE_CMD_ENTRANCE_LIST(inn_bedroom_scene_header04_entranceList),
    SCENE_CMD_SPAWN_LIST(1, inn_bedroom_scene_header04_playerEntryList),
    SCENE_CMD_ENV_LIGHT_SETTINGS(1, inn_bedroom_scene_header04_lightSettings),
    SCENE_CMD_TRANSITION_ACTOR_LIST(1, inn_bedroom_scene_header04_transitionActors),
    SCENE_CMD_CUTSCENE_DATA(TitleScreenCutscene),
    SCENE_CMD_END(),
};

ActorEntry inn_bedroom_scene_header04_playerEntryList[] = {
    {
        /* Actor ID   */ ACTOR_PLAYER,
        /* Position   */ { -170, -97, -177 },
        /* Rotation   */ { DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000), DEG_TO_BINANG(0.000) },
        /* Parameters */ 0x0FFF
    },
};

TransitionActorEntry inn_bedroom_scene_header04_transitionActors[] = {
    // Wooden Door
    {
        /* Room & Cam Index (Front, Back) */ { 0, 0x00, 0, 0x00 },
        /* Actor ID                       */ ACTOR_EN_DOOR,
        /* Position                       */ { -172, -97, -209 },
        /* Rotation Y                     */ DEG_TO_BINANG(0.000),
        /* Parameters                     */ 0x0000
    },
};

Spawn inn_bedroom_scene_header04_entranceList[] = {
    // { Spawn Actor List Index, Room Index }
    { 0, 0 },
};

EnvLightSettings inn_bedroom_scene_header04_lightSettings[1] = {
    // Indoor No. 1 Lighting
    {
        {    32,    41,    50 },   // Ambient Color
        {    73,    73,    73 },   // Diffuse0 Direction
        {     0,     0,     0 },   // Diffuse0 Color
        {   -73,   -73,   -73 },   // Diffuse1 Direction
        {     0,     0,     0 },   // Diffuse1 Color
        {   196,   182,   176 },   // Fog Color
        ((1 << 10) | 993),         // Blend Rate & Fog Near
        12800,                     // Fog Far
    },
};

