/**
 * Original LightsOut actor
 * random initial state, must bring state to all-0
 */

#include "lights_out.h"

#include "assets/loa_common.inc.c"
#include "assets/loa_sized.inc.c"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void LightsOut_Init(Actor* thisx, PlayState* play);
void LightsOut_Destroy(Actor* thisx, PlayState* play);
void LightsOut_Update(Actor* thisx, PlayState* play);
void LightsOut_Draw(Actor* thisx, PlayState* play);

ActorInit Lights_Out_InitVars = {
    /**/ ACTOR_LIGHTS_OUT,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(LightsOut),
    /**/ LightsOut_Init,
    /**/ LightsOut_Destroy,
    /**/ LightsOut_Update,
    /**/ LightsOut_Draw,
};

#include "lo_common.inc.c"

/**
 * Generate an initial state that is guaranteed to be solvable.
 *
 * A state is solvable (a set of moves exists that transforms this state to the winning state) if and only if the vector
 * representing the grid state is orthogonal to the generators of "quiet patterns", move combinations that leave the
 * board state unchanged.
 *
 * Anderson, M., & Feil, T. (1998).
 * Turning Lights Out with Linear Algebra.
 * Mathematics Magazine, 71(4), 300–303.
 * https://doi.org/10.2307/2690705
 *
 * https://people.sc.fsu.edu/~jburkardt/classes/imps_2017/11_28/2690705.pdf
 */
static u32 LightsOut_GenSolvableState(LightsOut* this, PlayState* play) {
    u8 matrix[LIGHTSOUT_GRID_XY * LIGHTSOUT_GRID_XY];

    // Build the matrix that describes the linear system
    LightsOut_BuildActionMatrix(matrix, LIGHTSOUT_GRID_X, LIGHTSOUT_GRID_Y);
#ifdef SOLVER_DEBUG
    Matrix_Print(matrix, LIGHTSOUT_GRID_XY);
#endif

    // Put it in reduced row echelon form (assumes LIGHTSOUT_NUM_STATES prime so that we work over the field GF(p))
    Matrix_GaussJordan(matrix, LIGHTSOUT_GRID_XY, LIGHTSOUT_NUM_STATES);
#ifdef SOLVER_DEBUG
    Matrix_Print(matrix, LIGHTSOUT_GRID_XY);
#endif

    // Determine number of null space basis vectors
    u32 dimNullR = 0;
    for (u32 i = 0; i < LIGHTSOUT_GRID_XY; i++) {
        u32 j = LIGHTSOUT_GRID_XY - 1 - i;
        if (matrix[LIGHTSOUT_GRID_XY * j + j] == 1) {
            break;
        }
        // Emplace a 1 in the diagonal for checking later
        matrix[LIGHTSOUT_GRID_XY * j + j] = 1;
        dimNullR++;
    }

#ifdef SOLVER_DEBUG
    rmonPrintf("dimNullR = %u\n", dimNullR);
#endif

    u32 numTries = 0;
retry:
    numTries++;

    // Generate an initial random state
    for (u32 i = 0; i < LIGHTSOUT_GRID_XY; i++) {
        this->gridArray[i] = ((u64)Rand_Next() * LIGHTSOUT_NUM_STATES) >> 32;
    }

#ifdef SOLVER_DEBUG
    Vector_Print(this->gridArray, LIGHTSOUT_GRID_XY);
#endif

    // Check solvability of the generated state
    for (u32 i = 0; i < dimNullR; i++) {
        // Get pointer to column containing a null vector (R is column-major)
        u8* nullVec = &matrix[LIGHTSOUT_GRID_XY * (LIGHTSOUT_GRID_XY - 1 - i)];

#ifdef SOLVER_DEBUG
        Vector_Print(nullVec, LIGHTSOUT_GRID_XY);
#endif

        // Compute inner product with null vector
        u32 inner = Mod_Inner(this->gridArray, nullVec, LIGHTSOUT_GRID_XY, LIGHTSOUT_NUM_STATES);

        // If inner product is not zero for any of these, the state is unsolvable and we need to re-roll
        if (inner != 0) {
#ifdef SOLVER_DEBUG
            rmonPrintf("RETRY =====\n");
#endif
            goto retry;
        }
    }

    return numTries;
}

void LightsOut_Init(Actor* thisx, PlayState* play) {
    LightsOut* this = (LightsOut*)thisx;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&gLightsOutCollision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    assert(this->dyna.bgId != BG_ACTOR_MAX);

    OSTime t = osGetTime();
    u32 numTries = LightsOut_GenSolvableState(this, play);
    t = osGetTime() - t;
    rmonPrintf("Took %u tries, %llu usec\n", numTries, OS_CYCLES_TO_USEC(t));

    this->lastPlayerCellX = 0xFF;
    this->lastPlayerCellZ = 0xFF;
    this->inert = false;
    this->overlayAlpha = 32;
}

void LightsOut_Destroy(Actor* thisx, PlayState* play) {
    LightsOut* this = (LightsOut*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

// (model space cell length) * (actor scale 0.01)
#define CELL_SIDE_X ((50000.0f / LIGHTSOUT_GRID_X) * 0.01f)
#define CELL_SIDE_Z ((50000.0f / LIGHTSOUT_GRID_Y) * 0.01f)

static s32 LightsOut_GetPlayerCell(LightsOut* this, PlayState* play, u8* x, u8* z) {
    if (!func_800435B4(&this->dyna)) {
        return false;
    }

    Player* player = GET_PLAYER(play);

    f32 cosY = Math_CosS(this->dyna.actor.shape.rot.y);
    f32 sinY = Math_SinS(this->dyna.actor.shape.rot.y);
    f32 deltaX = player->actor.world.pos.x - this->dyna.actor.world.pos.x;
    f32 deltaZ = player->actor.world.pos.z - this->dyna.actor.world.pos.z;

    f32 px = deltaX * cosY - deltaZ * sinY;
    f32 pz = deltaX * sinY + deltaZ * cosY;

    px += CELL_SIDE_X * (f32)LIGHTSOUT_GRID_X / 2;
    pz += CELL_SIDE_Z * (f32)LIGHTSOUT_GRID_Y / 2;

    s32 cellX = (s32)(px / CELL_SIDE_X);
    s32 cellZ = (s32)(pz / CELL_SIDE_Z);

    if (cellX < 0 || cellX > LIGHTSOUT_GRID_X - 1) {
        return false;
    }
    if (cellZ < 0 || cellZ > LIGHTSOUT_GRID_Y - 1) {
        return false;
    }
    *x = cellX;
    *z = cellZ;
    return true;
}

static void LightsOut_SingleCellAdvance(LightsOut* this, u8 x, u8 y) {
    if (LIGHTSOUT_NUM_STATES == 2) {
        this->grid[y][x] ^= 1;
    } else {
        this->grid[y][x] = (this->grid[y][x] + 1) % LIGHTSOUT_NUM_STATES;
    }
}

static void LightsOut_StateAdvance(LightsOut* this, PlayState* play, u8 x, u8 z) {
    // Advance cell state
    LightsOut_SingleCellAdvance(this, x, z);

    // Also update direct neighbors
    if (x > 0) {
        LightsOut_SingleCellAdvance(this, x - 1, z);
    }
    if (x < LIGHTSOUT_GRID_X - 1) {
        LightsOut_SingleCellAdvance(this, x + 1, z);
    }
    if (z > 0) {
        LightsOut_SingleCellAdvance(this, x, z - 1);
    }
    if (z < LIGHTSOUT_GRID_Y - 1) {
        LightsOut_SingleCellAdvance(this, x, z + 1);
    }
}

static s32 LightsOut_CheckWinCondition(LightsOut* this, PlayState* play) {
    for (u32 i = 0; i < LIGHTSOUT_GRID_XY; i++) {
        if (this->gridArray[i] != 0) {
            return false;
        }
    }
    return true;
}

void LightsOut_Update(Actor* thisx, PlayState* play) {
    LightsOut* this = (LightsOut*)thisx;

    if (this->inert) {
        if (this->overlayAlpha != 0) {
            this->overlayAlpha--;
        }
        return;
    }

    // Update state

    u8 cellX;
    u8 cellZ;
    if (LightsOut_GetPlayerCell(this, play, &cellX, &cellZ)) {
        if (cellX != this->lastPlayerCellX || cellZ != this->lastPlayerCellZ) {
            // Player is in a cell that is different from before
            LightsOut_StateAdvance(this, play, cellX, cellZ);

            this->lastPlayerCellX = cellX;
            this->lastPlayerCellZ = cellZ;

            if (LightsOut_CheckWinCondition(this, play)) {
                // Game is in winning state
                this->inert = true;
                Audio_PlaySfxGeneral(NA_SE_SY_CORRECT_CHIME, &gSfxDefaultPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }
        }
    } else {
        this->lastPlayerCellX = 0xFF;
        this->lastPlayerCellZ = 0xFF;
    }
}

void LightsOut_Draw(Actor* thisx, PlayState* play) {
    LightsOut* this = (LightsOut*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    // Just incase...
    osWritebackDCache(this->grid, sizeof(this->grid));

    gSPSegment(POLY_OPA_DISP++, 0x08, this->grid);
    gSPSegment(POLY_OPA_DISP++, 0x09, sLightsOutBoardPalette);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, this->overlayAlpha);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, LIGHTSOUT_SIZED_ASSET_NAME(gLightsOutBoardDL));

    CLOSE_DISPS(play->state.gfxCtx);
}
