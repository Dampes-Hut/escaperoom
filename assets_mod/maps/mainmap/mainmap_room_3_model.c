#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "mainmap_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

Gfx mainmap_room_3_entry_0_opaque[] = {
	gsSPDisplayList(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque),
	gsSPEndDisplayList(),
};

u64 mainmap_dl_spot07_room_0Tex_004748_rgba16_ci8[] = {
	0x0001020304050606, 0x0708090909070a0b, 0x0c0d0e0f10050606, 0x0708090909070a0b, 0x0c0d0e0f10050606, 0x0708090909070a0b, 0x0c0d0e0f10050606, 0x0708091112131401, 
	0x1500041617180519, 0x0505091a091b190a, 0x021c1c0e0f180519, 0x0505091a091b190a, 0x021c1c0e0f180519, 0x0505091a091b190a, 0x021c1c0e0f180519, 0x0505091d1e1f1520, 
	0x0121220014231c24, 0x1905050909240706, 0x0519180f25231c24, 0x1905050909240706, 0x0519180f25231c24, 0x1905050909240706, 0x0519180f25231c24, 0x1905102627282901, 
	0x2a2b2c2d150e102e, 0x0a0505070807062f, 0x080702180a0e102e, 0x0a0505070807062f, 0x080702180a0e102e, 0x0a0505070807062f, 0x080702180a0e102e, 0x0a0530122d223101, 
	0x3215330434300235, 0x3637051907062f38, 0x380807241b050235, 0x3637051907062f38, 0x380807241b050235, 0x3637051907062f38, 0x380807241b050235, 0x363705393a3b3c00, 
	0x2c1e21263d301c3e, 0x3f400a0a411b2f38, 0x38092f1b1b051c3e, 0x3f400a0a411b2f38, 0x38092f1b1b051c3e, 0x3f400a0a411b2f38, 0x38092f1b1b051c3e, 0x3f400a4243393720, 
	0x0004221e00440a2e, 0x4546470a0a190708, 0x093809091b2f0a2e, 0x4546470a0a190708, 0x093809091b2f0a2e, 0x4546470a0a190708, 0x093809091b2f0a2e, 0x4546473a48024901, 
	0x20224a174b1b0908, 0x4c4d4d400a0a1905, 0x081b0909091b0908, 0x4c4d4d400a0a1905, 0x081b0909091b0908, 0x4c4d4d400a0a1905, 0x081b0909091b0908, 0x4c4d4d2022211501, 
	0x4e134f00041a0806, 0x0a504d0c40510a41, 0x050709091a1a0806, 0x0a504d0c40510a41, 0x050709091a1a0806, 0x0a504d0c40510a41, 0x050709091a1a0806, 0x0a504d5204532d01, 
	0x1517061e161a1b08, 0x07514d540c4c1905, 0x410507091a1a1b08, 0x07514d540c4c1905, 0x410507091a1a1b08, 0x07514d540c4c1905, 0x410507091a1a1b08, 0x07514d2648062e01, 
	0x001d1e2655120956, 0x2407514740500d06, 0x0741071b09570956, 0x2407514740500d06, 0x0741071b09570956, 0x2407514740500d06, 0x0741071b09570956, 0x240751583a485900, 
	0x015a275b391b1a5c, 0x090705510b5d5e35, 0x2e07191b091b1a5c, 0x090705510b5d5e35, 0x2e07191b091b1a5c, 0x090705510b5d5e35, 0x2e07191b091b1a5c, 0x0907051d06493d00, 
	0x063c2c5f39095c57, 0x1a08092e4c355e60, 0x3506070609095c57, 0x1a08092e4c355e60, 0x3506070609095c57, 0x1a08092e4c355e60, 0x3506070609095c57, 0x1a08090455290101, 
	0x6100624942091a63, 0x1a0909071951353e, 0x0b0e190507091a63, 0x1a0909071951353e, 0x0b0e190507091a63, 0x1a0909071951353e, 0x0b0e190507091a63, 0x1a09093d03282015, 
	0x005f2662222e0956, 0x56091b0907414c2e, 0x3f3564640a190956, 0x56091b0907414c2e, 0x3f3564640a190956, 0x56091b0907414c2e, 0x3f3564640a190956, 0x5609654f22224815, 
	0x15222c2255070724, 0x0909090909054140, 0x661c183605070724, 0x0909090909054140, 0x661c183605070724, 0x0909090909054140, 0x661c183605070724, 0x090909322a164f4e, 
	
};

u64 mainmap_dl_spot07_room_0Tex_004748_rgba16_pal_rgba16[] = {
	0x7b9783d9a4dfbda9, 0x9c9f40038c193801, 0x839528014803bda3, 0x618f590950c7514b, 0x48c710455a4f945d, 0x6ad37b5739cb528f, 0x5045945b20013001, 0x50875a8fc5eb949d, 
	0x8c1bb5a73989b5a1, 0x83d758c9314941cb, 0xb5657b5573553189, 0x8c5ba5239c9d7b11, 0x518b9cdd7357bdeb, 0x0003ad214045ad1f, 0x72cf524f3147ce6d, 0x5a916b15b561618d, 
	0x590ba4dd9ca17b95, 0x6ad1d6656a955889, 0xb5674a0dad6562d3, 0x584569cf7b994a4d, 0xce255003b5a95ad1, 0x6a536b55628d5a0d, 0x394bad23c62d6291, 0x49cb6251d625c62b, 
	0xc5e57315bde93107, 0x4845520dc5e30000
};

Vtx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {316, -249, 1409}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {316, 158, 1409}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {316, 158, 639}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {316, -249, 639}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1040, -249, 1409}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1040, 158, 1409}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1040, 158, 639}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1040, -249, 639}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_0[101] = {
	{{ {1039, 37, 1149}, 0, {3035, 1217}, {3, 166, 90, 255} }},
	{{ {1040, 37, 1209}, 0, {3035, 1563}, {253, 166, 166, 255} }},
	{{ {1030, 37, 1210}, 0, {2949, 1563}, {140, 218, 221, 255} }},
	{{ {1029, 37, 1150}, 0, {2949, 1217}, {142, 218, 42, 255} }},
	{{ {1040, -63, 1209}, 0, {3035, 1563}, {253, 90, 166, 255} }},
	{{ {1039, -63, 1149}, 0, {3035, 1217}, {3, 90, 90, 255} }},
	{{ {1029, -63, 1150}, 0, {2949, 1217}, {185, 73, 76, 255} }},
	{{ {1030, -63, 1210}, 0, {2949, 1563}, {180, 73, 185, 255} }},
	{{ {1040, 37, 1209}, 0, {3035, -239}, {253, 166, 166, 255} }},
	{{ {1040, -63, 1209}, 0, {3035, 339}, {253, 90, 166, 255} }},
	{{ {1030, -63, 1210}, 0, {2949, 339}, {180, 73, 185, 255} }},
	{{ {1030, 37, 1210}, 0, {2949, -239}, {140, 218, 221, 255} }},
	{{ {1039, -63, 1149}, 0, {3035, 339}, {3, 90, 90, 255} }},
	{{ {1039, 37, 1149}, 0, {3035, -239}, {3, 166, 90, 255} }},
	{{ {1029, 37, 1150}, 0, {2949, -239}, {142, 218, 42, 255} }},
	{{ {1029, -63, 1150}, 0, {2949, 339}, {185, 73, 76, 255} }},
	{{ {1031, -149, 927}, 0, {3036, 831}, {253, 166, 166, 255} }},
	{{ {1031, -249, 927}, 0, {3036, 1412}, {253, 90, 166, 255} }},
	{{ {1021, -249, 927}, 0, {2950, 1412}, {180, 73, 185, 255} }},
	{{ {1021, -149, 927}, 0, {2950, 831}, {140, 218, 221, 255} }},
	{{ {1031, -249, 927}, 0, {3036, -69}, {253, 90, 166, 255} }},
	{{ {1029, -249, 867}, 0, {3036, -416}, {3, 90, 90, 255} }},
	{{ {1019, -249, 867}, 0, {2950, -416}, {185, 73, 76, 255} }},
	{{ {1021, -249, 927}, 0, {2950, -69}, {180, 73, 185, 255} }},
	{{ {1029, -149, 867}, 0, {3036, -416}, {3, 166, 90, 255} }},
	{{ {1031, -149, 927}, 0, {3036, -69}, {253, 166, 166, 255} }},
	{{ {1021, -149, 927}, 0, {2950, -69}, {140, 218, 221, 255} }},
	{{ {1019, -149, 867}, 0, {2950, -416}, {142, 218, 42, 255} }},
	{{ {1029, -249, 867}, 0, {3036, 1412}, {3, 90, 90, 255} }},
	{{ {1029, -149, 867}, 0, {3036, 831}, {3, 166, 90, 255} }},
	{{ {1019, -149, 867}, 0, {2950, 831}, {142, 218, 42, 255} }},
	{{ {1019, -249, 867}, 0, {2950, 1412}, {185, 73, 76, 255} }},
	{{ {1027, -249, 1091}, 0, {2950, 1412}, {252, 0, 129, 255} }},
	{{ {331, -249, 1114}, 0, {-3020, 1412}, {252, 0, 129, 255} }},
	{{ {331, -63, 1114}, 0, {-3020, 339}, {252, 0, 129, 255} }},
	{{ {1027, -63, 1091}, 0, {2950, 339}, {252, 0, 129, 255} }},
	{{ {316, -249, 662}, 0, {-3020, 1412}, {4, 0, 127, 255} }},
	{{ {1012, -249, 639}, 0, {2950, 1412}, {4, 0, 127, 255} }},
	{{ {1012, -63, 639}, 0, {2950, 339}, {4, 0, 127, 255} }},
	{{ {316, -63, 662}, 0, {-3020, 339}, {4, 0, 127, 255} }},
	{{ {1012, 55, 639}, 0, {2950, -349}, {4, 0, 127, 255} }},
	{{ {316, 55, 662}, 0, {-3020, -349}, {4, 0, 127, 255} }},
	{{ {1012, 158, 639}, 0, {2950, -942}, {4, 0, 127, 255} }},
	{{ {316, 158, 662}, 0, {-3020, -942}, {4, 0, 127, 255} }},
	{{ {1019, -149, 867}, 0, {2085, 831}, {142, 218, 42, 255} }},
	{{ {1012, -249, 639}, 0, {4041, 1412}, {129, 0, 4, 255} }},
	{{ {1019, -249, 867}, 0, {2085, 1412}, {185, 73, 76, 255} }},
	{{ {1012, -63, 639}, 0, {4041, 339}, {129, 0, 4, 255} }},
	{{ {1027, -63, 1091}, 0, {164, 339}, {129, 0, 4, 255} }},
	{{ {1021, -149, 927}, 0, {1570, 831}, {140, 218, 221, 255} }},
	{{ {331, -249, 1114}, 0, {164, 1412}, {127, 0, 252, 255} }},
	{{ {316, -249, 662}, 0, {4041, 1412}, {127, 0, 252, 255} }},
	{{ {316, -63, 662}, 0, {4041, 339}, {127, 0, 252, 255} }},
	{{ {331, -63, 1114}, 0, {164, 339}, {127, 0, 252, 255} }},
	{{ {331, -63, 1114}, 0, {164, 339}, {127, 0, 252, 255} }},
	{{ {316, 55, 662}, 0, {4041, -349}, {127, 0, 252, 255} }},
	{{ {329, 55, 1036}, 0, {833, -349}, {127, 0, 252, 255} }},
	{{ {316, 158, 662}, 0, {4041, -942}, {127, 0, 252, 255} }},
	{{ {331, 158, 1114}, 0, {164, -942}, {127, 0, 252, 255} }},
	{{ {341, -63, 1409}, 0, {-2365, 339}, {127, 0, 252, 255} }},
	{{ {331, -63, 1114}, 0, {164, 339}, {127, 0, 252, 255} }},
	{{ {329, 55, 1036}, 0, {833, -349}, {127, 0, 252, 255} }},
	{{ {341, 55, 1409}, 0, {-2365, -349}, {127, 0, 252, 255} }},
	{{ {1030, 37, 1210}, 0, {-852, -239}, {140, 218, 221, 255} }},
	{{ {1030, -63, 1210}, 0, {-852, 339}, {180, 73, 185, 255} }},
	{{ {1036, -63, 1386}, 0, {-2365, 339}, {129, 0, 4, 255} }},
	{{ {1036, 55, 1386}, 0, {-2365, -349}, {129, 0, 4, 255} }},
	{{ {1012, -63, 639}, 0, {4041, 339}, {129, 0, 4, 255} }},
	{{ {1027, -63, 1091}, 0, {164, 339}, {129, 0, 4, 255} }},
	{{ {1024, 55, 1013}, 0, {833, -349}, {129, 0, 4, 255} }},
	{{ {1012, 55, 639}, 0, {4041, -349}, {129, 0, 4, 255} }},
	{{ {1036, -63, 1386}, 0, {2949, 339}, {252, 0, 129, 255} }},
	{{ {341, -63, 1409}, 0, {-3020, 339}, {252, 0, 129, 255} }},
	{{ {341, 55, 1409}, 0, {-3020, -349}, {252, 0, 129, 255} }},
	{{ {1036, 55, 1386}, 0, {2949, -349}, {252, 0, 129, 255} }},
	{{ {341, 104, 1409}, 0, {-3020, -628}, {252, 0, 129, 255} }},
	{{ {1036, 104, 1386}, 0, {2949, -628}, {252, 0, 129, 255} }},
	{{ {341, 55, 1409}, 0, {-2365, -349}, {127, 0, 252, 255} }},
	{{ {329, 55, 1036}, 0, {833, -349}, {127, 0, 252, 255} }},
	{{ {331, 158, 1114}, 0, {164, -942}, {127, 0, 252, 255} }},
	{{ {341, 104, 1409}, 0, {-2365, -628}, {127, 0, 252, 255} }},
	{{ {1024, 55, 1013}, 0, {833, -349}, {129, 0, 4, 255} }},
	{{ {1036, 55, 1386}, 0, {-2365, -349}, {129, 0, 4, 255} }},
	{{ {1036, 104, 1386}, 0, {-2365, -628}, {129, 0, 4, 255} }},
	{{ {1027, 158, 1091}, 0, {164, -942}, {129, 0, 4, 255} }},
	{{ {1012, 55, 639}, 0, {4041, -349}, {129, 0, 4, 255} }},
	{{ {1024, 55, 1013}, 0, {833, -349}, {129, 0, 4, 255} }},
	{{ {1027, 158, 1091}, 0, {164, -942}, {129, 0, 4, 255} }},
	{{ {1012, 158, 639}, 0, {4041, -942}, {129, 0, 4, 255} }},
	{{ {1021, -249, 927}, 0, {1570, 1412}, {180, 73, 185, 255} }},
	{{ {1027, -249, 1091}, 0, {164, 1412}, {129, 0, 4, 255} }},
	{{ {1027, -63, 1091}, 0, {164, 339}, {129, 0, 4, 255} }},
	{{ {1021, -149, 927}, 0, {1570, 831}, {140, 218, 221, 255} }},
	{{ {1029, 37, 1150}, 0, {-338, -239}, {142, 218, 42, 255} }},
	{{ {1036, 55, 1386}, 0, {-2365, -349}, {129, 0, 4, 255} }},
	{{ {1029, 37, 1150}, 0, {-338, -239}, {142, 218, 42, 255} }},
	{{ {1036, 55, 1386}, 0, {-2365, -349}, {129, 0, 4, 255} }},
	{{ {1024, 55, 1013}, 0, {833, -349}, {129, 0, 4, 255} }},
	{{ {1029, -63, 1150}, 0, {-338, 339}, {185, 73, 76, 255} }},
	{{ {1024, 55, 1013}, 0, {833, -349}, {129, 0, 4, 255} }},
	{{ {1027, -63, 1091}, 0, {164, 339}, {129, 0, 4, 255} }},
};

Gfx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_0 + 0, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
	gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
	gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
	gsSP2Triangles(28, 29, 30, 0, 28, 30, 31, 0),
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_0 + 32, 31, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(7, 6, 8, 0, 7, 8, 9, 0),
	gsSP2Triangles(9, 8, 10, 0, 9, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 15, 13, 0),
	gsSP2Triangles(12, 16, 15, 0, 12, 17, 16, 0),
	gsSP2Triangles(18, 19, 20, 0, 18, 20, 21, 0),
	gsSP2Triangles(22, 20, 23, 0, 22, 23, 24, 0),
	gsSP2Triangles(24, 23, 25, 0, 24, 25, 26, 0),
	gsSP2Triangles(27, 28, 29, 0, 27, 29, 30, 0),
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_0 + 63, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(11, 10, 12, 0, 11, 12, 13, 0),
	gsSP2Triangles(14, 15, 16, 0, 14, 16, 17, 0),
	gsSP2Triangles(18, 19, 20, 0, 18, 20, 21, 0),
	gsSP2Triangles(22, 23, 24, 0, 22, 24, 25, 0),
	gsSP2Triangles(26, 27, 28, 0, 26, 28, 29, 0),
	gsSP1Triangle(30, 0, 31, 0),
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_0 + 95, 6, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 0, 4, 0),
	gsSP1Triangle(3, 4, 5, 0),
	gsSPEndDisplayList(),
};

Vtx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_1[8] = {
	{{ {331, -249, 1114}, 0, {-10614, 1773}, {0, 127, 0, 255} }},
	{{ {1027, -249, 1091}, 0, {5477, 1773}, {0, 127, 0, 255} }},
	{{ {1012, -249, 639}, 0, {5477, -3451}, {0, 127, 0, 255} }},
	{{ {316, -249, 662}, 0, {-10614, -3451}, {0, 127, 0, 255} }},
	{{ {341, -63, 1409}, 0, {-10614, 5181}, {0, 127, 0, 255} }},
	{{ {1036, -63, 1386}, 0, {5474, 5181}, {0, 127, 0, 255} }},
	{{ {1027, -63, 1091}, 0, {5477, 1773}, {0, 127, 0, 255} }},
	{{ {331, -63, 1114}, 0, {-10614, 1773}, {0, 127, 0, 255} }},
};

Gfx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_tri_1[] = {
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_1 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Vtx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_2[8] = {
	{{ {1036, 104, 1386}, 0, {5474, 5083}, {255, 131, 233, 255} }},
	{{ {341, 104, 1409}, 0, {-10614, 5083}, {255, 131, 233, 255} }},
	{{ {331, 158, 1114}, 0, {-10614, 1722}, {255, 131, 233, 255} }},
	{{ {1027, 158, 1091}, 0, {5477, 1722}, {255, 131, 233, 255} }},
	{{ {316, 158, 662}, 0, {-15890, -5243}, {0, 129, 0, 255} }},
	{{ {1012, 158, 639}, 0, {8174, -5243}, {0, 129, 0, 255} }},
	{{ {1027, 158, 1091}, 0, {8173, 2570}, {0, 129, 0, 255} }},
	{{ {331, 158, 1114}, 0, {-15890, 2570}, {0, 129, 0, 255} }},
};

Gfx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_tri_2[] = {
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_2 + 0, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Gfx mat_mainmap_dl_placeholder_ballroom_wall_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_LIGHTING_POSITIONAL | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_RGBA16 | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, mainmap_dl_spot07_room_0Tex_004748_rgba16_pal_rgba16),
	gsDPSetTile(0, 0, 0, 256, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadTLUTCmd(5, 102),
	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 1, mainmap_dl_spot07_room_0Tex_004748_rgba16_ci8),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 511, 256),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 4, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 60),
	gsSPEndDisplayList(),
};

Gfx mat_mainmap_dl_placeholder_ballroom_floor_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_LIGHTING_POSITIONAL | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_RGBA16 | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 201, 154, 99, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, mainmap_dl_hylia_labo_sceneTex_00B090_rgba16_pal_rgba16),
	gsDPSetTile(0, 0, 0, 256, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadTLUTCmd(5, 96),
	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 1, mainmap_dl_hylia_labo_sceneTex_00B090_rgba16_ci8),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPEndDisplayList(),
};

Gfx mat_mainmap_dl_placeholder_ballroom_ceiling_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_LIGHTING_POSITIONAL | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_RGBA16 | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, mainmap_dl_hylia_labo_sceneTex_00B090_rgba16_pal_rgba16),
	gsDPSetTile(0, 0, 0, 256, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadTLUTCmd(5, 96),
	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 1, mainmap_dl_hylia_labo_sceneTex_00B090_rgba16_ci8),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 8, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0),
	gsDPSetTileSize(0, 0, 0, 252, 124),
	gsSPEndDisplayList(),
};

Gfx mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_mainmap_dl_placeholder_ballroom_wall_layerOpaque),
	gsSPDisplayList(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_tri_0),
	gsSPDisplayList(mat_mainmap_dl_placeholder_ballroom_floor_layerOpaque),
	gsSPDisplayList(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_tri_1),
	gsSPDisplayList(mat_mainmap_dl_placeholder_ballroom_ceiling_layerOpaque),
	gsSPDisplayList(mainmap_dl_ballroom_quick_geo_mesh_layer_Opaque_tri_2),
	gsSPEndDisplayList(),
};

