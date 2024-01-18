#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "mainmap_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

Gfx mainmap_room_1_entry_0_opaque[] = {
	gsSPDisplayList(mainmap_dl_towerfloor_mesh_layer_Opaque),
	gsSPEndDisplayList(),
};

Vtx mainmap_dl_towerfloor_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {327, -151, -55}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {327, -151, -55}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {327, -151, -318}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {327, -151, -318}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {575, -151, -55}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {575, -151, -55}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {575, -151, -318}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {575, -151, -318}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx mainmap_dl_towerfloor_mesh_layer_Opaque_vtx_0[4] = {
	{{ {438, -151, -55}, 0, {1765, 1075}, {0, 127, 0, 255} }},
	{{ {575, -151, -204}, 0, {-802, 2304}, {0, 127, 0, 255} }},
	{{ {464, -151, -318}, 0, {-802, 2304}, {0, 127, 0, 255} }},
	{{ {327, -151, -170}, 0, {1765, 1075}, {0, 127, 0, 255} }},
};

Gfx mainmap_dl_towerfloor_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(mainmap_dl_towerfloor_mesh_layer_Opaque_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mainmap_dl_towerfloor_mesh_layer_Opaque[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(mainmap_dl_towerfloor_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_mainmap_dl_step_layerOpaque),
	gsSPDisplayList(mainmap_dl_towerfloor_mesh_layer_Opaque_tri_0),
	gsSPEndDisplayList(),
};

