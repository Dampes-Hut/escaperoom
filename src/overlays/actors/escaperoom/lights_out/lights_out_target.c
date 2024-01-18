/**
 * Target LightsOut actor, generates a random target state shown on the board.
 * Player can't interact with it (walking on cells doesn't change state)
 * Works in tandem with dancefloor LightsOut actor lights_out_fancedloor
 */

#include "lights_out_target.h"

#include "assets/loa_common.inc.c"
#include "assets/loa_sized.inc.c"
#include "lo_common.inc.c"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void LightsOutTarget_Init(Actor* thisx, PlayState* play);
void LightsOutTarget_Destroy(Actor* thisx, PlayState* play);
void LightsOutTarget_Update(Actor* thisx, PlayState* play);
void LightsOutTarget_Draw(Actor* thisx, PlayState* play);

ActorInit Lights_Out_Target_InitVars = {
    /**/ ACTOR_LIGHTS_OUT_TARGET,
    /**/ ACTORCAT_BG,
    /**/ FLAGS,
    /**/ OBJECT_GAMEPLAY_KEEP,
    /**/ sizeof(LightsOutTarget),
    /**/ LightsOutTarget_Init,
    /**/ LightsOutTarget_Destroy,
    /**/ LightsOutTarget_Update,
    /**/ LightsOutTarget_Draw,
};

static u8 sTargetGridMin[LIGHTSOUT_GRID_Y][LIGHTSOUT_GRID_X] = {
    { 0, 0, 0, 0, 0, 4, 0 },
    { 2, 2, 2, 2, 2, 4, 4 },
    { 0, 0, 0, 0, 0, 4, 0 },
};
static u8 sTargetGridMax[LIGHTSOUT_GRID_Y][LIGHTSOUT_GRID_X] = {
    { 1, 1, 1, 1, 1, 4, 1 },
    { 3, 3, 3, 3, 3, 4, 4 },
    { 1, 1, 1, 1, 1, 4, 1 },
};

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
static u32 LightsOutTarget_GenSolvableState(LightsOutTarget* this, PlayState* play) {
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

    u32 seed;
    if (gSaveContext.save.info.playerData.is_dance_floor_seed_chosen) {
        // if a seed has been chosen it was valid, we shouldn't need to "try" more than once
        assert(numTries == 1);

        seed = gSaveContext.save.info.playerData.dance_floor_seed;
    } else {
        // no seed chosen, this is the initial generation, generate a random seed every try
        seed = Rand_Next();
    }
    u32 rng = seed;

    // Generate a random target state
    for (u32 i = 0; i < LIGHTSOUT_GRID_XY; i++) {
        this->grid.as_1d[i] =
            ((u8*)sTargetGridMin)[i] +
            (((u64)Rand_Next_Variable(&rng) * (((u8*)sTargetGridMax)[i] - ((u8*)sTargetGridMin)[i] + 1)) >> 32);
    }

    // checkSolveGridArray is checked if it can be solved as in
    // "with checkSolveGridArray as initial state, can the state be brought to all 0"
    // However we want to check the opposite, that is,
    // "with an all-0 initial state, can the state be brought to this->gridArray"
    // This is equivalent to:
    // checkSolveGridArray = -this->gridArray (mod p)
    u8 checkSolveGridArray[LIGHTSOUT_GRID_XY];
    for (u32 i = 0; i < LIGHTSOUT_GRID_XY; i++) {
        checkSolveGridArray[i] = (LIGHTSOUT_NUM_STATES - this->grid.as_1d[i]) % LIGHTSOUT_NUM_STATES;
    }

#ifdef SOLVER_DEBUG
    Vector_Print(checkSolveGridArray, LIGHTSOUT_GRID_XY);
#endif

    // Check solvability of the generated state
    for (u32 i = 0; i < dimNullR; i++) {
        // Get pointer to column containing a null vector (R is column-major)
        u8* nullVec = &matrix[LIGHTSOUT_GRID_XY * (LIGHTSOUT_GRID_XY - 1 - i)];

#ifdef SOLVER_DEBUG
        Vector_Print(nullVec, LIGHTSOUT_GRID_XY);
#endif

        // Compute inner product with null vector
        u32 inner = Mod_Inner(checkSolveGridArray, nullVec, LIGHTSOUT_GRID_XY, LIGHTSOUT_NUM_STATES);

        // If inner product is not zero for any of these, the state is unsolvable and we need to re-roll
        if (inner != 0) {
#ifdef SOLVER_DEBUG
            rmonPrintf("RETRY =====\n");
#endif
            goto retry;
        }
    }

    if (!gSaveContext.save.info.playerData.is_dance_floor_seed_chosen) {
        gSaveContext.save.info.playerData.dance_floor_seed = seed;
        gSaveContext.save.info.playerData.is_dance_floor_seed_chosen = true;
        rmonPrintf("is_dance_floor_seed_chosen = true\n");
    } else {
        // if a seed was already chosen, we should have used it above
        assert(gSaveContext.save.info.playerData.dance_floor_seed == seed);
    }
    rmonPrintf("gSaveContext.save.info.playerData.dance_floor_seed = 0x%08X\n",
               gSaveContext.save.info.playerData.dance_floor_seed);

    return numTries;
}

void LightsOutTarget_Init(Actor* thisx, PlayState* play) {
    LightsOutTarget* this = (LightsOutTarget*)thisx;
    CollisionHeader* colHeader = NULL;

    this->dyna.actor.scale = (Vec3f){ LIGHTSOUT_SCALE_X, 0.01f, LIGHTSOUT_SCALE_Y };

    DynaPolyActor_Init(&this->dyna, 0);
    CollisionHeader_GetVirtual(&gLightsOutCollision, &colHeader);
    this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
    assert(this->dyna.bgId != BG_ACTOR_MAX);

    OSTime t = osGetTime();
    u32 numTries = LightsOutTarget_GenSolvableState(this, play);
    t = osGetTime() - t;
    rmonPrintf("Took %u tries, %llu usec\n", numTries, OS_CYCLES_TO_USEC(t));

#if 1
    for (int y = 0; y < LIGHTSOUT_GRID_Y; y++) {
        for (int x = 0; x < LIGHTSOUT_GRID_X; x++) {
            rmonPrintf("%d ", (int)this->grid.as_2d[y][x]);
        }
        rmonPrintf("\n");
    }
#endif

    this->overlayAlpha = 50;
}

void LightsOutTarget_Destroy(Actor* thisx, PlayState* play) {
    LightsOutTarget* this = (LightsOutTarget*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
}

void LightsOutTarget_Update(Actor* thisx, PlayState* play) {
    LightsOutTarget* this = (LightsOutTarget*)thisx;
}

void LightsOutTarget_Draw(Actor* thisx, PlayState* play) {
    LightsOutTarget* this = (LightsOutTarget*)thisx;

    OPEN_DISPS(play->state.gfxCtx);

    // Just incase...
    osWritebackDCache(&this->grid, sizeof(this->grid));

    gSPSegment(POLY_OPA_DISP++, 0x08, &this->grid);
    gSPSegment(POLY_OPA_DISP++, 0x09, sLightsOutBoardPalette);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, this->overlayAlpha);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPDisplayList(POLY_OPA_DISP++, LIGHTSOUT_SIZED_ASSET_NAME(gLightsOutBoardDL));

    CLOSE_DISPS(play->state.gfxCtx);
}
