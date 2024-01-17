#include "ultra64.h"
#include "z64.h"
#include "macros.h"
#include "test_dancefloor_scene.h"
#include "segment_symbols.h"
#include "command_macros_base.h"
#include "z64cutscene_commands.h"
#include "variables.h"

SurfaceType test_dancefloor_polygonTypes[] = {
	{ 0x00000000, 0x00000000 },
};

CollisionPoly test_dancefloor_polygons[] = {
	{ 0x0000, 0x0000, 0x0001, 0x0002, COLPOLY_SNORMAL(0.0), COLPOLY_SNORMAL(1.0), COLPOLY_SNORMAL(7.549790126404332e-08), 0x0078 },
	{ 0x0000, 0x0000, 0x0002, 0x0003, COLPOLY_SNORMAL(0.0), COLPOLY_SNORMAL(1.0), COLPOLY_SNORMAL(7.549790126404332e-08), 0x0078 },
};

Vec3s test_dancefloor_vertices[4] = {
	{ -822, -120, 822 },
	{ 822, -120, 822 },
	{ 822, -120, -822 },
	{ -822, -120, -822 },
};

CollisionHeader test_dancefloor_collisionHeader = {
	-822,
	-120,
	-822,
	822,
	-120,
	822,
	4,
	test_dancefloor_vertices,
	2,
	test_dancefloor_polygons,
	test_dancefloor_polygonTypes,
	0,
	0,
	0
};

