#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "mainmap_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

Gfx mainmap_room_5_entry_0_opaque[] = {
	gsSPDisplayList(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque),
	gsSPEndDisplayList(),
};

u64 mainmap_dl_ganontika_room_1Tex_0129E0_ci8_ci8[] = {
	0x0000010102030403, 0x0500030306060602, 0x0206060203030007, 0x0706080809020200, 0x040404040404040a, 0x05070506060b0606, 0x0606020004040405, 0x0504030606060406, 
	0x0c0d0e0f10070710, 0x110e121007050707, 0x0713131310111410, 0x1007070705050707, 0x1516171819171a0d, 0x1b1a1c170c1d1a0d, 0x07061e191f190d16, 0x08201d211e1f1e04, 
	0x08200003081e1b01, 0x2200000819000819, 0x1023241b06010008, 0x0003001e001e0007, 0x252600010008030b, 0x0c0c030008030808, 0x0402010027000200, 0x0100080800080007, 
	0x1b0d0b0008190808, 0x130d06060d06020d, 0x061b010027030b08, 0x030d030000000013, 0x010c0100081e0800, 0x060b020209020b09, 0x0400010027000008, 0x0009000100010007, 
	0x25030b0000080000, 0x06280203010c010d, 0x040106090d1b0029, 0x080903060806082a, 0x1e082b0008080000, 0x0200000b0d020101, 0x0408030b0800081e, 0x000806020801000c, 
	0x081e2c0308000302, 0x020613130c04100d, 0x0700270100000901, 0x001b000201020105, 0x0c082c0300030306, 0x02132a2c0b2a1309, 0x05021b0109091b08, 0x000300022701092d, 
	0x131e00001b000002, 0x02062806012a0600, 0x0502270808080800, 0x080302030002030c, 0x050809030f060606, 0x06061306022c0602, 0x05100b0c27010000, 0x00000c010601040c, 
	0x0502090d27270106, 0x0606040704040704, 0x0510010b03060404, 0x0604040202020204, 0x0713130713131313, 0x2a07070707070707, 0x0c0c141007070707, 0x0713040707070713, 
	0x2e09161f171e1d19, 0x171e02002f1b1d1b, 0x101b2f21212f091b, 0x251100080921211c, 0x29001e0800030001, 0x1e08040829080b00, 0x000008000008001d, 0x180215000808081b, 
	0x0102021306131302, 0x0202071b1e080606, 0x0602000602000001, 0x0004130900020000, 0x0c0c13132c2a2c02, 0x0406070108030003, 0x0c060b0c06010802, 0x1b04060000060206, 
	0x1d1e1b0c0b062828, 0x2a13070008080006, 0x2a01081300080808, 0x212c062701060101, 0x0108080000030606, 0x0b13070000000302, 0x00020b00020b0001, 0x1b0400270b081d08, 
	0x0108080003030306, 0x012a053003010201, 0x062a13062a13000b, 0x0907090c01010900, 0x00001e0003010606, 0x01020708030b0202, 0x062a0c062a0c0b13, 0x0c05010608020000, 
	0x0000001b0008000b, 0x0601050b060c0613, 0x0604040604040604, 0x06050f0b01060c03, 0x0300000800000006, 0x0406050206020606, 0x0602060602060202, 0x020509090c000809, 
	0x0301000b03030606, 0x0606050602060206, 0x0602030602030b00, 0x1305130c0b000808, 0x0707060606060c06, 0x13060507020c0c0c, 0x0c0b0c0c0b0c0713, 0x0705130713060606, 
	0x0505050507070705, 0x0505070707070407, 0x0404050404050505, 0x0505070707050505, 0x1d08000106020b30, 0x303030271e1d1931, 0x322f19030c291627, 0x27331d1e00010b1a, 
	0x08081e1e17221e1d, 0x341d340008000003, 0x020300002c330908, 0x0800080808000300, 0x0002032717021e01, 0x0101010101060206, 0x020102032c1d0b01, 0x0c02270009030303, 
	0x0800020b352c0802, 0x0002000100080102, 0x0206033528080206, 0x0303000003030300, 0x0001020c08070006, 0x0006000800000303, 0x0201023605080602, 0x0303000000030003, 
	0x00000b0200050302, 0x0302032b062b0301, 0x0000000028000202, 0x0303030003030303, 0x0003000003050006, 0x0606060602020603, 0x030300022a150001, 0x0203060102030000, 
	0x0206010007130107, 0x0407040306060203, 0x0302000007000003, 0x0008030300020003, 0x02020201070c062c, 0x032c032c04020200, 0x0008032b05010001, 0x0303030300030000, 
	0x06060200070b0100, 0x000000032c000109, 0x00000013050c020b, 0x0303030303000001, 0x06060b0c07000300, 0x0100032302042c06, 0x0704061305070b01, 0x010b0b0000080106, 
	0x0705072a07040604, 0x3704370607050507, 0x07070707072a0707, 0x05130c0713130407, 0x000900001e380002, 0x070505041313031d, 0x1e39191d2f1a1b1e, 0x0800030706290800, 
	0x291e1e3a00001e19, 0x0006013b292c3c17, 0x080c060606011306, 0x03080603090d1b19, 0x0103020606060602, 0x010c0102002c090b, 0x0604130c13060602, 0x02082c1e000c0b03, 
	0x0206130606281300, 0x00060b0208040902, 0x062c0406042a0504, 0x131b131d010b0606, 0x1307040606030302, 0x02040c0208041b08, 0x030b070206062804, 0x1300071e01010407, 
	0x1304130606060707, 0x020c010200040901, 0x0102030106042c07, 0x061305080306042a, 0x13072a2a06070202, 0x0113060700050306, 0x0206060707072c2a, 0x1306050106061306, 
	0x02072a0704070406, 0x130607050b282c03, 0x03060c2a042a2a07, 0x2a06050b07070200, 0x020622060c060607, 0x2a06050504050903, 0x0304020c02022a2a, 0x0706050001060413, 
	0x0606000001020207, 0x0707050506050600, 0x06020d0207130600, 0x040307000602022c, 0x0901000d020c0c07, 0x2c0203282a050403, 0x01010b0602040102, 0x0602280206060008, 
	0x27272727270c0d01, 0x220b030203050406, 0x2a07062a07050713, 0x0606050506020100, 0x1410070705051304, 0x0604070505050705, 0x0707050507040407, 0x0501101010070c14, 
	0x1a1a1b1e0b070705, 0x05050713220d193d, 0x393e19091d1d0113, 0x071e171d1b0c1a21, 0x0200000000010008, 0x10000816170d0908, 0x08081e08080c1e00, 0x0c1e000306060b00, 
	0x060c06021b081919, 0x0729001b000c0606, 0x0603000206030002, 0x072900010c13130c, 0x0100000800000829, 0x0c1b080001020106, 0x0000000000000603, 0x050006010b060602, 
	0x060006000300001a, 0x0601060604040606, 0x020000000200020c, 0x0a001313060c0602, 0x0203030000080100, 0x05030b0613060606, 0x0306020606020606, 0x050004060c060402, 
	0x060008083f1e083a, 0x0706060602060606, 0x060604020606010c, 0x05000c020c06040c, 0x06021e0800080003, 0x0703060202020200, 0x000000000800130c, 0x0708000006042a06, 
	0x01000b0302020006, 0x0503030203060b06, 0x0200000b09000c00, 0x101e0000060c130c, 0x0600020302000303, 0x0502060306022a2a, 0x070b02032a020202, 0x05030600010b0c03, 
	
};

u64 mainmap_dl_ganontika_room_1Tex_0129E0_ci8_pal_rgba16[] = {
	0x418d3189290939c9, 0x18c50083210708c5, 0x41cf39cf00412949, 0x2149298f29d1218f, 0x094908cf198f1907, 0x088d59c962937295, 0x92996a534a1149d1, 0x82955a1359d17359, 
	0x6b595a53198d5a07, 0x69c94a535a993a11, 0x0103629109053987, 0x190518c18b9b7255, 0x41d38ad1624f5a55, 0x420d5a0949c72903, 0x6a136a0d5a0d4a95, 0x4a516a91820f3153, 
	
};

Vtx mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {447, -359, 1538}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {447, -127, 1538}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {447, -127, 1148}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {447, -359, 1148}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1044, -359, 1538}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1044, -127, 1538}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1044, -127, 1148}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {1044, -359, 1148}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_0[101] = {
	{{ {447, -127, 1538}, 0, {-3797, 6825}, {0, 129, 0, 255} }},
	{{ {447, -127, 1304}, 0, {-3797, 1551}, {0, 129, 0, 255} }},
	{{ {825, -127, 1538}, 0, {482, 6825}, {0, 129, 0, 255} }},
	{{ {825, -127, 1148}, 0, {482, -1990}, {0, 129, 0, 255} }},
	{{ {447, -127, 1244}, 0, {-3797, 195}, {0, 129, 0, 255} }},
	{{ {447, -127, 1148}, 0, {-3797, -1990}, {0, 129, 0, 255} }},
	{{ {988, -249, 1304}, 0, {2329, 1551}, {0, 127, 0, 255} }},
	{{ {1034, -249, 1304}, 0, {2841, 1551}, {0, 127, 0, 255} }},
	{{ {1034, -249, 1244}, 0, {2841, 195}, {0, 127, 0, 255} }},
	{{ {988, -249, 1244}, 0, {2329, 195}, {0, 127, 0, 255} }},
	{{ {1042, -149, 1244}, 0, {2932, 188}, {3, 166, 90, 255} }},
	{{ {1044, -149, 1304}, 0, {2954, 1543}, {253, 166, 166, 255} }},
	{{ {1034, -149, 1304}, 0, {2841, 1551}, {255, 167, 166, 255} }},
	{{ {1032, -149, 1244}, 0, {2819, 195}, {1, 166, 90, 255} }},
	{{ {1042, -249, 1244}, 0, {2932, 1398}, {3, 90, 90, 255} }},
	{{ {1042, -149, 1244}, 0, {2932, -873}, {3, 166, 90, 255} }},
	{{ {1032, -149, 1244}, 0, {2819, -873}, {1, 166, 90, 255} }},
	{{ {1032, -249, 1244}, 0, {2819, 1398}, {3, 90, 90, 255} }},
	{{ {1044, -149, 1304}, 0, {2954, -873}, {253, 166, 166, 255} }},
	{{ {1044, -249, 1304}, 0, {2954, 1398}, {253, 90, 166, 255} }},
	{{ {1034, -249, 1304}, 0, {2841, 1398}, {255, 90, 166, 255} }},
	{{ {1034, -149, 1304}, 0, {2841, -873}, {255, 167, 166, 255} }},
	{{ {1044, -249, 1304}, 0, {2954, 1543}, {253, 90, 166, 255} }},
	{{ {1042, -249, 1244}, 0, {2932, 188}, {3, 90, 90, 255} }},
	{{ {1032, -249, 1244}, 0, {2819, 195}, {3, 90, 90, 255} }},
	{{ {1034, -249, 1304}, 0, {2841, 1551}, {255, 90, 166, 255} }},
	{{ {988, -249, 1244}, 0, {2329, 1398}, {0, 0, 127, 255} }},
	{{ {1034, -249, 1244}, 0, {2841, 1398}, {0, 0, 127, 255} }},
	{{ {1032, -149, 1244}, 0, {2819, -873}, {0, 0, 127, 255} }},
	{{ {960, -149, 1244}, 0, {2003, -873}, {0, 0, 127, 255} }},
	{{ {960, -149, 1244}, 0, {2003, 195}, {0, 129, 0, 255} }},
	{{ {1032, -149, 1244}, 0, {2819, 195}, {0, 129, 0, 255} }},
	{{ {1034, -149, 1304}, 0, {2841, 1551}, {0, 129, 0, 255} }},
	{{ {961, -149, 1304}, 0, {2025, 1551}, {0, 129, 0, 255} }},
	{{ {961, -149, 1304}, 0, {2025, -873}, {0, 0, 129, 255} }},
	{{ {1034, -149, 1304}, 0, {2841, -873}, {0, 0, 129, 255} }},
	{{ {1034, -249, 1304}, 0, {2841, 1398}, {0, 0, 129, 255} }},
	{{ {988, -249, 1304}, 0, {2329, 1398}, {0, 0, 129, 255} }},
	{{ {960, -149, 1244}, 0, {2098, 189}, {72, 151, 254, 255} }},
	{{ {961, -149, 1304}, 0, {2120, 1544}, {72, 151, 254, 255} }},
	{{ {825, -242, 1304}, 0, {579, 1544}, {72, 151, 254, 255} }},
	{{ {823, -242, 1244}, 0, {557, 189}, {72, 151, 254, 255} }},
	{{ {961, -149, 1304}, 0, {2025, -873}, {0, 0, 129, 255} }},
	{{ {825, -359, 1304}, 0, {482, 3884}, {0, 0, 129, 255} }},
	{{ {825, -242, 1304}, 0, {482, 1234}, {0, 0, 129, 255} }},
	{{ {988, -249, 1304}, 0, {2329, 1551}, {185, 105, 0, 255} }},
	{{ {988, -249, 1244}, 0, {2329, 195}, {185, 105, 0, 255} }},
	{{ {825, -359, 1244}, 0, {482, 195}, {185, 105, 0, 255} }},
	{{ {825, -359, 1304}, 0, {482, 1551}, {185, 105, 0, 255} }},
	{{ {988, -249, 1244}, 0, {2329, 1398}, {0, 0, 127, 255} }},
	{{ {960, -149, 1244}, 0, {2003, -873}, {0, 0, 127, 255} }},
	{{ {823, -242, 1244}, 0, {460, 1234}, {0, 0, 127, 255} }},
	{{ {825, -359, 1244}, 0, {482, 3884}, {0, 0, 127, 255} }},
	{{ {825, -359, 1304}, 0, {482, 1551}, {0, 127, 0, 255} }},
	{{ {825, -359, 1244}, 0, {482, 195}, {0, 127, 0, 255} }},
	{{ {447, -359, 1244}, 0, {-3797, 195}, {0, 127, 0, 255} }},
	{{ {447, -359, 1304}, 0, {-3797, 1551}, {0, 127, 0, 255} }},
	{{ {447, -359, 1304}, 0, {-3797, 1551}, {0, 127, 0, 255} }},
	{{ {447, -359, 1538}, 0, {-3797, 6825}, {0, 127, 0, 255} }},
	{{ {825, -359, 1538}, 0, {482, 6825}, {0, 127, 0, 255} }},
	{{ {447, -359, 1244}, 0, {-3797, 195}, {0, 127, 0, 255} }},
	{{ {825, -359, 1148}, 0, {482, -1990}, {0, 127, 0, 255} }},
	{{ {447, -359, 1244}, 0, {-3797, 195}, {0, 127, 0, 255} }},
	{{ {825, -359, 1148}, 0, {482, -1990}, {0, 127, 0, 255} }},
	{{ {447, -359, 1148}, 0, {-3797, -1990}, {0, 127, 0, 255} }},
	{{ {447, -359, 1244}, 0, {513, 3884}, {127, 0, 0, 255} }},
	{{ {447, -359, 1148}, 0, {1606, 3884}, {127, 0, 0, 255} }},
	{{ {447, -127, 1148}, 0, {1606, -1360}, {127, 0, 0, 255} }},
	{{ {447, -127, 1244}, 0, {513, -1360}, {127, 0, 0, 255} }},
	{{ {447, -359, 1538}, 0, {-2802, 3884}, {127, 0, 0, 255} }},
	{{ {447, -359, 1304}, 0, {-164, 3884}, {127, 0, 0, 255} }},
	{{ {447, -127, 1304}, 0, {-164, -1360}, {127, 0, 0, 255} }},
	{{ {447, -127, 1538}, 0, {-2802, -1360}, {127, 0, 0, 255} }},
	{{ {825, -359, 1538}, 0, {482, 3884}, {0, 0, 129, 255} }},
	{{ {447, -359, 1538}, 0, {-3797, 3884}, {0, 0, 129, 255} }},
	{{ {447, -127, 1538}, 0, {-3797, -1360}, {0, 0, 129, 255} }},
	{{ {825, -127, 1538}, 0, {482, -1360}, {0, 0, 129, 255} }},
	{{ {447, -359, 1148}, 0, {-3797, 3884}, {0, 0, 127, 255} }},
	{{ {825, -359, 1148}, 0, {482, 3884}, {0, 0, 127, 255} }},
	{{ {825, -127, 1148}, 0, {482, -1360}, {0, 0, 127, 255} }},
	{{ {447, -127, 1148}, 0, {-3797, -1360}, {0, 0, 127, 255} }},
	{{ {447, -359, 1304}, 0, {-164, 3884}, {127, 0, 0, 255} }},
	{{ {447, -359, 1244}, 0, {513, 3884}, {127, 0, 0, 255} }},
	{{ {447, -127, 1244}, 0, {513, -1360}, {127, 0, 0, 255} }},
	{{ {447, -127, 1304}, 0, {-164, -1360}, {127, 0, 0, 255} }},
	{{ {823, -242, 1244}, 0, {513, 1234}, {129, 255, 254, 255} }},
	{{ {825, -127, 1148}, 0, {1605, -1360}, {129, 255, 254, 255} }},
	{{ {825, -359, 1148}, 0, {1605, 3883}, {129, 255, 254, 255} }},
	{{ {825, -359, 1244}, 0, {513, 3884}, {129, 255, 254, 255} }},
	{{ {825, -359, 1304}, 0, {-164, 3884}, {129, 0, 0, 255} }},
	{{ {825, -359, 1538}, 0, {-2802, 3884}, {129, 0, 0, 255} }},
	{{ {825, -127, 1538}, 0, {-2802, -1360}, {129, 0, 0, 255} }},
	{{ {825, -242, 1304}, 0, {-164, 1234}, {129, 0, 0, 255} }},
	{{ {825, -242, 1304}, 0, {-164, 1234}, {129, 1, 1, 255} }},
	{{ {825, -127, 1538}, 0, {-2802, -1360}, {129, 1, 1, 255} }},
	{{ {825, -127, 1148}, 0, {1606, -1360}, {129, 1, 1, 255} }},
	{{ {823, -242, 1244}, 0, {513, 1234}, {129, 1, 1, 255} }},
	{{ {447, -127, 1304}, 0, {-3797, 1551}, {0, 129, 0, 255} }},
	{{ {447, -127, 1244}, 0, {-3797, 195}, {0, 129, 0, 255} }},
	{{ {825, -127, 1148}, 0, {482, -1990}, {0, 129, 0, 255} }},
	{{ {825, -127, 1538}, 0, {482, 6825}, {0, 129, 0, 255} }},
};

Gfx mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_0 + 0, 30, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(6, 7, 8, 0, 6, 8, 9, 0),
	gsSP2Triangles(10, 11, 12, 0, 10, 12, 13, 0),
	gsSP2Triangles(14, 15, 16, 0, 14, 16, 17, 0),
	gsSP2Triangles(18, 19, 20, 0, 18, 20, 21, 0),
	gsSP2Triangles(22, 23, 24, 0, 22, 24, 25, 0),
	gsSP2Triangles(26, 27, 28, 0, 26, 28, 29, 0),
	gsSPVertex(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_0 + 30, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 7, 13, 0, 12, 13, 14, 0),
	gsSP2Triangles(15, 16, 17, 0, 15, 17, 18, 0),
	gsSP2Triangles(19, 20, 21, 0, 19, 21, 22, 0),
	gsSP2Triangles(23, 24, 25, 0, 23, 25, 26, 0),
	gsSP2Triangles(23, 27, 28, 0, 23, 28, 29, 0),
	gsSP1Triangle(30, 24, 31, 0),
	gsSPVertex(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_0 + 62, 31, 0),
	gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
	gsSP2Triangles(3, 5, 6, 0, 7, 8, 9, 0),
	gsSP2Triangles(7, 9, 10, 0, 11, 12, 13, 0),
	gsSP2Triangles(11, 13, 14, 0, 15, 16, 17, 0),
	gsSP2Triangles(15, 17, 18, 0, 19, 20, 21, 0),
	gsSP2Triangles(19, 21, 22, 0, 23, 24, 25, 0),
	gsSP2Triangles(23, 25, 26, 0, 27, 28, 29, 0),
	gsSP1Triangle(27, 29, 30, 0),
	gsSPVertex(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_0 + 93, 8, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSPEndDisplayList(),
};

Gfx mat_mainmap_dl_f3dlite_material_003_layerOpaque[] = {
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_LIGHTING_POSITIONAL | G_SHADING_SMOOTH),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_RGBA16 | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, mainmap_dl_ganontika_room_1Tex_0129E0_ci8_pal_rgba16),
	gsDPSetTile(0, 0, 0, 256, 5, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadTLUTCmd(5, 63),
	gsDPSetTextureImage(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 1, mainmap_dl_ganontika_room_1Tex_0129E0_ci8_ci8),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 512),
	gsDPSetTile(G_IM_FMT_CI, G_IM_SIZ_8b, 4, 0, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 6, 0, G_TX_WRAP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 252),
	gsSPEndDisplayList(),
};

Gfx mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_mainmap_dl_f3dlite_material_003_layerOpaque),
	gsSPDisplayList(mainmap_dl_forbiddenlibrary_quick_geo_mesh_layer_Opaque_tri_0),
	gsSPEndDisplayList(),
};
