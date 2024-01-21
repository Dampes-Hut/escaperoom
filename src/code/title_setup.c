#include "global.h"

void Setup_MainImpl(SetupState* this) {
    osSyncPrintf("ゼルダ共通データ初期化\n"); // "Zelda common data initalization"
    SaveContext_Init();
    this->state.running = false;

#if !LOAD_SAVE_AND_MAPSELECT_ON_BOOT
    // vanilla
    SET_NEXT_GAMESTATE(&this->state, ConsoleLogo_Init, ConsoleLogoState);
#else
    Sram_InitDebugSave();

    // load
    Sram_InitSram(&this->state, &this->sramCtx);
    gSaveContext.fileNum = 1;
    Sram_OpenSave(&this->sramCtx);

    // loose check for if we loaded garbage (no existing save) or not
    char expected_newf[6] = "ZELDAZ";
    bool is_expected_newf = true;
    for (int i = 0; i < 6; i++) {
        if (gSaveContext.save.info.playerData.newf[i] != expected_newf[i]) {
            is_expected_newf = false;
        }
    }
    // if garbage, init and write a debug save
    if (!is_expected_newf) {
        rmonPrintf("!is_expected_newf\n");
        Sram_InitDebugSave();

        // save
        gSaveContext.fileNum = 1;
        Sram_WriteSave(&this->sramCtx);
    }

    SET_NEXT_GAMESTATE(&this->state, MapSelect_Init, MapSelectState);
#endif
}

void Setup_Main(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    Setup_MainImpl(this);
}

void Setup_Destroy(GameState* thisx) {
}

void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    this->state.main = Setup_Main;
    this->state.destroy = Setup_Destroy;

#if LOAD_SAVE_AND_MAPSELECT_ON_BOOT
    Sram_Alloc(&this->state, &this->sramCtx);
#endif
}
