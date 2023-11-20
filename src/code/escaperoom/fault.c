/**
 * @file fault.c
 *
 * This file implements the screen that may be viewed when the game crashes.
 * This is the second version of the crash screen, originally used in Majora's Mask.
 *
 * When the game crashes, a red bar will be drawn to the top-left of the screen, indicating that the
 * crash screen is available for use. Once this bar appears, it is possible to open the crash screen
 * with the following button combination:
 *
 * (L & R & Z) + DPad-Up + C-Down + C-Up + DPad-Down + DPad-Left + C-Left + C-Right + DPad-Right + (B & A & START)
 *
 * When entering this button combination, buttons that are &'d together must all be pressed together.
 * The L & R presses and B & A presses may be interchanged in the order they are pressed.
 *
 * "Clients" may be registered with the crash screen to extend its functionality. There are
 * two kinds of client, "Client" and "AddressConverterClient". Clients contribute one or
 * more pages to the crash debugger, while Address Converter Clients allow the crash screen to look up
 * the virtual addresses of dynamically allocated overlays.
 *
 * The crash screen has multiple pages:
 *  - Thread Context
 *      This page shows information about the thread on which the program crashed. It displays
 *      the cause of the crash, state of general-purpose registers, state of floating-point registers
 *      and the floating-point status register. If a floating-point exception caused the crash, it will
 *      be displayed next to the floating-point status register.
 *  - Stack Trace
 *      This page displays a full backtrace from the crashing function back to the start of the thread. It
 *      displays the Program Counter for each function and, if applicable, the Virtual Program Counter
 *      for relocated functions in overlays.
 *  - Client Pages
 *      After the stack trace page, currently registered clients are processed and their pages are displayed.
 *  - Memory Dump
 *      This page implements a scrollable memory dump.
 *  - End Screen
 *      This page informs you that there are no more pages to display.
 *
 * To navigate the pages, START and A may be used to advance to the next page, and L toggles whether to
 * automatically scroll to the next page after some time has passed.
 * DPad-Up may be pressed to enable sending fault pages over rmonPrintf as well as displaying them on-screen.
 * DPad-Down disables sending fault pages over rmonPrintf.
 */
#include "global.h"
#include "debug/disasm.h"
#include "terminal.h"
#include "alloca.h"

extern const char gBuildGitVersion[];

void FaultDrawer_Init(void);
void FaultDrawer_SetOsSyncPrintfEnabled(u32 enabled);
void FaultDrawer_DrawRecImpl(s32 xStart, s32 yStart, s32 xEnd, s32 yEnd, u16 color);
void FaultDrawer_FillScreen(void);
void FaultDrawer_SetInputCallback(s32 (*callback)(void));
void FaultDrawer_SetDrawerFB(void* fb, u16 w, u16 h);

const char* sExceptionNames[] = {
    "Interrupt",
    "TLB modification",
    "TLB exception on load",
    "TLB exception on store",
    "Address error on load",
    "Address error on store",
    "Bus error on inst.",
    "Bus error on data",
    "System call exception",
    "Breakpoint exception",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap exception",
    "Virtual coherency on inst.",
    "Floating point exception",
    "Watchpoint exception",
    "Virtual coherency on data",
};

const char* sFpExceptionNames[] = {
    "Unimplemented operation", "Invalid operation", "Division by zero", "Overflow", "Underflow", "Inexact operation",
};

u8 sIsHungup = false;
u8 sRestartable = false;

FaultMgr* sFaultInstance;
u8 sFaultAwaitingInput;
STACK(sFaultStack, 0x600);
StackEntry sFaultThreadInfo;
FaultMgr gFaultMgr;


typedef struct {
    /* 0x00 */ s32 (*callback)(void*, void*);
    /* 0x04 */ void* arg0;
    /* 0x08 */ void* arg1;
    /* 0x0C */ s32 ret;
    /* 0x10 */ OSMesgQueue* queue;
    /* 0x14 */ OSMesg msg;
} FaultClientTask; // size = 0x18

void Fault_SleepImpl(u32 msec) {
    u64 cycles = (msec * OS_CPU_COUNTER) / 1000ull;

    Sleep_Cycles(cycles);
}

void Fault_ClientRunTask(void* arg) {
    FaultClientTask* task = (FaultClientTask*)arg;

    // Run the callback
    if (task->callback != NULL) {
        task->ret = task->callback(task->arg0, task->arg1);
    }
}

/**
 * Registers a fault client.
 *
 * Clients contribute at least one page to the crash screen, drawn by `callback`.
 * Arguments are passed on to the callback through `arg0` and `arg1`.
 *
 * The callback is intended to be
 * `s32 (*callback)(void* arg0, void* arg1)`
 */
void Fault_AddClient(FaultClient* client, s32 (*callback)(void*, void*), void* arg0, void* arg1) {
    OSIntMask mask;
    s32 alreadyExists = false;

    mask = osSetIntMask(OS_IM_NONE);

    // Ensure the client is not already registered
    {
        FaultClient* iterClient = sFaultInstance->clients;

        while (iterClient != NULL) {
            if (iterClient == client) {
                alreadyExists = true;
                goto end;
            }
            iterClient = iterClient->next;
        }
    }

    client->callback = callback;
    client->arg0 = arg0;
    client->arg1 = arg1;
    client->next = sFaultInstance->clients;
    sFaultInstance->clients = client;

end:
    osSetIntMask(mask);
    if (alreadyExists) {
        rmonPrintf(VT_COL(RED, WHITE) "fault_AddClient: %08x is already registered!\n" VT_RST, client);
    }
}

/**
 * Removes a fault client so that the page is no longer displayed if a crash occurs.
 */
void Fault_RemoveClient(FaultClient* client) {
    FaultClient* iterClient = sFaultInstance->clients;
    FaultClient* lastClient = NULL;
    OSIntMask mask;
    s32 listIsEmpty = false;

    mask = osSetIntMask(OS_IM_NONE);

    while (iterClient != NULL) {
        if (iterClient == client) {
            if (lastClient != NULL) {
                lastClient->next = client->next;
            } else {
                sFaultInstance->clients = client;
                if (sFaultInstance->clients) {
                    sFaultInstance->clients = client->next;
                } else {
                    listIsEmpty = true;
                }
            }
            break;
        }
        lastClient = iterClient;
        iterClient = iterClient->next;
    }

    osSetIntMask(mask);

    if (listIsEmpty) {
        rmonPrintf(VT_COL(RED, WHITE) "fault_RemoveClient: %08x is not registered!\n" VT_RST, client);
    }
}

/**
 * Registers an address converter client. This enables the crash screen to look up virtual
 * addresses of overlays relocated during runtime. Address conversion is carried out by
 * `callback`, which either returns a virtual address or NULL if the address could not
 * be converted.
 *
 * The callback is intended to be
 * `uintptr_t (*callback)(uintptr_t addr, void* arg)`
 * The callback may return 0 if it could not convert the address
 * The callback may return -1 to be unregistered
 */
void Fault_AddAddrConvClient(FaultAddrConvClient* client, uintptr_t (*callback)(uintptr_t, void*), void* arg) {
    OSIntMask mask;
    s32 alreadyExists = false;

    mask = osSetIntMask(OS_IM_NONE);

    // Ensure the client is not already registered
    {
        FaultAddrConvClient* iterClient = sFaultInstance->addrConvClients;

        while (iterClient != NULL) {
            if (iterClient == client) {
                alreadyExists = true;
                goto end;
            }
            iterClient = iterClient->next;
        }
    }

    client->callback = callback;
    client->arg = arg;
    client->next = sFaultInstance->addrConvClients;
    sFaultInstance->addrConvClients = client;

end:
    osSetIntMask(mask);
    if (alreadyExists) {
        rmonPrintf(VT_COL(RED, WHITE) "fault_AddressConverterAddClient: %08x is already registered!\n" VT_RST, client);
    }
}

void Fault_RemoveAddrConvClient(FaultAddrConvClient* client) {
    FaultAddrConvClient* iterClient = sFaultInstance->addrConvClients;
    FaultAddrConvClient* lastClient = NULL;
    OSIntMask mask;
    s32 listIsEmpty = false;

    mask = osSetIntMask(OS_IM_NONE);

    while (iterClient != NULL) {
        if (iterClient == client) {
            if (lastClient != NULL) {
                lastClient->next = client->next;
            } else {
                sFaultInstance->addrConvClients = client;

                if (sFaultInstance->addrConvClients != NULL) {
                    sFaultInstance->addrConvClients = client->next;
                } else {
                    listIsEmpty = true;
                }
            }
            break;
        }
        lastClient = iterClient;
        iterClient = iterClient->next;
    }

    osSetIntMask(mask);

    if (listIsEmpty) {
        rmonPrintf(VT_COL(RED, WHITE) "fault_AddressConverterRemoveClient: %08x is not registered!\n" VT_RST, client);
    }
}

/**
 * Converts `addr` to a virtual address via the registered
 * address converter clients
 */
uintptr_t Fault_ConvertAddress(uintptr_t addr) {
    FaultAddrConvClient* client;

    for (client = sFaultInstance->addrConvClients; client != NULL; client = client->next) {
        if (client->callback == NULL) {
            continue;
        }

        uintptr_t ret = client->callback(addr, client->arg);
        if (ret == (uintptr_t)(-1)) {
            Fault_RemoveAddrConvClient(client);
        } else if (ret != 0) {
            return ret;
        }
    }
    return (uintptr_t)NULL;
}

void Fault_Sleep(u32 msec) {
    Fault_SleepImpl(msec);
}

void Fault_PadCallback(Input* inputs) {
    PadMgr_RequestPadData(&gPadMgr, inputs, false);
}

void Fault_UpdatePadImpl(void) {
    sFaultInstance->padCallback(sFaultInstance->inputs);
}

/**
 * Awaits user input
 *
 * L toggles auto-scroll
 * DPad-Up enables rmonPrintf output
 * DPad-Down disables rmonPrintf output
 * A and DPad-Right continues and returns true
 * DPad-Left continues and returns false
 */
s32 Fault_WaitForInputImpl(void) {
    Input* input = &sFaultInstance->inputs[0];
    s32 count = 600;
    u32 pressedBtn;

    while (true) {
        Fault_Sleep(1000 / 60);
        Fault_UpdatePadImpl();

        pressedBtn = input->press.button;

        if (pressedBtn == BTN_L) {
            sFaultInstance->autoScroll = !sFaultInstance->autoScroll;
        }

        if (sFaultInstance->autoScroll) {
            if (count-- < 1) {
                return false;
            }
        } else {
            if (pressedBtn == BTN_A) {
                return false;
            }
            if (pressedBtn == BTN_START) {
                return sRestartable;
            }

            if (pressedBtn == BTN_DUP) {
                FaultDrawer_SetOsSyncPrintfEnabled(true);
            }

            if (pressedBtn == BTN_DDOWN) {
                FaultDrawer_SetOsSyncPrintfEnabled(false);
            }
        }
    }
}

s32 Fault_WaitForInput(void) {
    sFaultAwaitingInput = true;
    s32 restart = Fault_WaitForInputImpl();
    sFaultAwaitingInput = false;
    return restart;
}

void Fault_DrawRec(s32 x, s32 y, s32 w, s32 h, u16 color) {
    FaultDrawer_DrawRecImpl(x, y, x + w - 1, y + h - 1, color);
}

void Fault_FillScreenBlack(void) {
    FaultDrawer_SetForeColor(GPACK_RGBA5551(255, 255, 255, 1));
    FaultDrawer_SetBackColor(GPACK_RGBA5551(0, 0, 0, 1));
    FaultDrawer_FillScreen();
    FaultDrawer_SetBackColor(GPACK_RGBA5551(0, 0, 0, 0));
}

void Fault_FillScreenRed(void) {
    FaultDrawer_SetForeColor(GPACK_RGBA5551(255, 255, 255, 1));
    FaultDrawer_SetBackColor(GPACK_RGBA5551(240, 0, 0, 1));
    FaultDrawer_FillScreen();
    FaultDrawer_SetBackColor(GPACK_RGBA5551(0, 0, 0, 0));
}

void Fault_DrawCornerRec(u16 color) {
    Fault_DrawRec(22, 16, 8, 1, color);
}

void Fault_PrintFReg(s32 idx, f32* value) {
    u32 raw = *(u32*)value;
    s32 exp = ((raw & 0x7F800000) >> 0x17) - 0x7F;

    if ((exp >= -0x7E && exp < 0x80) || raw == 0) {
        FaultDrawer_Printf("F%02d:%14.7e ", idx, *value);
    } else {
        // Print subnormal floats as their ieee-754 hex representation
        FaultDrawer_Printf("F%02d:  %08x(16) ", idx, raw);
    }
}

void Fault_LogFReg(s32 idx, f32* value) {
    u32 raw = *(u32*)value;
    s32 exp = ((raw & 0x7F800000) >> 0x17) - 0x7F;

    if ((exp >= -0x7E && exp < 0x80) || raw == 0) {
        rmonPrintf("F%02d:%14.7e ", idx, *value);
    } else {
        rmonPrintf("F%02d:  %08x(16) ", idx, *(u32*)value);
    }
}

void Fault_PrintFPCSR(u32 value) {
    s32 i;
    u32 flag = FPCSR_CE;

    FaultDrawer_Printf("FPCSR:%08xH ", value);

    // Go through each of the six causes and print the name of
    // the first cause that is set
    for (i = 0; i < ARRAY_COUNT(sFpExceptionNames); i++) {
        if (value & flag) {
            FaultDrawer_Printf("(%s)", sFpExceptionNames[i]);
            break;
        }
        flag >>= 1;
    }
    FaultDrawer_Printf("\n");
}

void Fault_LogFPCSR(u32 value) {
    s32 i;
    u32 flag = FPCSR_CE;

    rmonPrintf("FPCSR:%08xH  ", value);
    for (i = 0; i < ARRAY_COUNT(sFpExceptionNames); i++) {
        if (value & flag) {
            rmonPrintf("(%s)\n", sFpExceptionNames[i]);
            break;
        }
        flag >>= 1;
    }
}

void Fault_PrintThreadContext(OSThread* thread) {
    __OSThreadContext* ctx;
    s16 causeStrIdx = _SHIFTR((u32)thread->context.cause, 2, 5);

    if (causeStrIdx == 23) { // Watchpoint
        causeStrIdx = 16;
    }
    if (causeStrIdx == 31) { // Virtual coherency on data
        causeStrIdx = 17;
    }

    FaultDrawer_FillScreen();
    FaultDrawer_SetCharPad(-2, 4);
    FaultDrawer_SetCursor(22, 18);

    ctx = &thread->context;
#ifdef PACKAGE_VERSION
    FaultDrawer_Printf("GIT COMMIT %s\n", gBuildGitVersion);
#endif
    FaultDrawer_Printf("THREAD:%d (%d:%s)\n", thread->id, causeStrIdx, sExceptionNames[causeStrIdx]);
    FaultDrawer_SetCharPad(-1, 0);

    FaultDrawer_Printf("PC:%08xH SR:%08xH VA:%08xH\n", (u32)ctx->pc, (u32)ctx->sr, (u32)ctx->badvaddr);
    FaultDrawer_Printf("AT:%08xH V0:%08xH V1:%08xH\n", (u32)ctx->at, (u32)ctx->v0, (u32)ctx->v1);
    FaultDrawer_Printf("A0:%08xH A1:%08xH A2:%08xH\n", (u32)ctx->a0, (u32)ctx->a1, (u32)ctx->a2);
    FaultDrawer_Printf("A3:%08xH T0:%08xH T1:%08xH\n", (u32)ctx->a3, (u32)ctx->t0, (u32)ctx->t1);
    FaultDrawer_Printf("T2:%08xH T3:%08xH T4:%08xH\n", (u32)ctx->t2, (u32)ctx->t3, (u32)ctx->t4);
    FaultDrawer_Printf("T5:%08xH T6:%08xH T7:%08xH\n", (u32)ctx->t5, (u32)ctx->t6, (u32)ctx->t7);
    FaultDrawer_Printf("S0:%08xH S1:%08xH S2:%08xH\n", (u32)ctx->s0, (u32)ctx->s1, (u32)ctx->s2);
    FaultDrawer_Printf("S3:%08xH S4:%08xH S5:%08xH\n", (u32)ctx->s3, (u32)ctx->s4, (u32)ctx->s5);
    FaultDrawer_Printf("S6:%08xH S7:%08xH T8:%08xH\n", (u32)ctx->s6, (u32)ctx->s7, (u32)ctx->t8);
    FaultDrawer_Printf("T9:%08xH GP:%08xH SP:%08xH\n", (u32)ctx->t9, (u32)ctx->gp, (u32)ctx->sp);
    FaultDrawer_Printf("S8:%08xH RA:%08xH LO:%08xH\n\n", (u32)ctx->s8, (u32)ctx->ra, (u32)ctx->lo);

    Fault_PrintFPCSR(ctx->fpcsr);
    FaultDrawer_Printf("\n");

    Fault_PrintFReg(0, &ctx->fp0.f.f_even);
    Fault_PrintFReg(2, &ctx->fp2.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(4, &ctx->fp4.f.f_even);
    Fault_PrintFReg(6, &ctx->fp6.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(8, &ctx->fp8.f.f_even);
    Fault_PrintFReg(10, &ctx->fp10.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(12, &ctx->fp12.f.f_even);
    Fault_PrintFReg(14, &ctx->fp14.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(16, &ctx->fp16.f.f_even);
    Fault_PrintFReg(18, &ctx->fp18.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(20, &ctx->fp20.f.f_even);
    Fault_PrintFReg(22, &ctx->fp22.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(24, &ctx->fp24.f.f_even);
    Fault_PrintFReg(26, &ctx->fp26.f.f_even);
    FaultDrawer_Printf("\n");
    Fault_PrintFReg(28, &ctx->fp28.f.f_even);
    Fault_PrintFReg(30, &ctx->fp30.f.f_even);

    FaultDrawer_SetCharPad(0, 0);
}

void Fault_LogThreadContext(OSThread* thread) {
    __OSThreadContext* ctx;
    s16 causeStrIdx = _SHIFTR((u32)thread->context.cause, 2, 5);

    if (causeStrIdx == 23) { // Watchpoint
        causeStrIdx = 16;
    }
    if (causeStrIdx == 31) { // Virtual coherency on data
        causeStrIdx = 17;
    }

    ctx = &thread->context;
    rmonPrintf("\n");
#ifdef PACKAGE_VERSION
    rmonPrintf("GIT COMMIT %s\n", gBuildGitVersion);
#endif
    rmonPrintf("THREAD ID:%d (%d:%s)\n", thread->id, causeStrIdx, sExceptionNames[causeStrIdx]);

    rmonPrintf("PC:%08xH   SR:%08xH   VA:%08xH\n", (u32)ctx->pc, (u32)ctx->sr, (u32)ctx->badvaddr);
    rmonPrintf("AT:%08xH   V0:%08xH   V1:%08xH\n", (u32)ctx->at, (u32)ctx->v0, (u32)ctx->v1);
    rmonPrintf("A0:%08xH   A1:%08xH   A2:%08xH\n", (u32)ctx->a0, (u32)ctx->a1, (u32)ctx->a2);
    rmonPrintf("A3:%08xH   T0:%08xH   T1:%08xH\n", (u32)ctx->a3, (u32)ctx->t0, (u32)ctx->t1);
    rmonPrintf("T2:%08xH   T3:%08xH   T4:%08xH\n", (u32)ctx->t2, (u32)ctx->t3, (u32)ctx->t4);
    rmonPrintf("T5:%08xH   T6:%08xH   T7:%08xH\n", (u32)ctx->t5, (u32)ctx->t6, (u32)ctx->t7);
    rmonPrintf("S0:%08xH   S1:%08xH   S2:%08xH\n", (u32)ctx->s0, (u32)ctx->s1, (u32)ctx->s2);
    rmonPrintf("S3:%08xH   S4:%08xH   S5:%08xH\n", (u32)ctx->s3, (u32)ctx->s4, (u32)ctx->s5);
    rmonPrintf("S6:%08xH   S7:%08xH   T8:%08xH\n", (u32)ctx->s6, (u32)ctx->s7, (u32)ctx->t8);
    rmonPrintf("T9:%08xH   GP:%08xH   SP:%08xH\n", (u32)ctx->t9, (u32)ctx->gp, (u32)ctx->sp);
    rmonPrintf("S8:%08xH   RA:%08xH   LO:%08xH\n", (u32)ctx->s8, (u32)ctx->ra, (u32)ctx->lo);
    rmonPrintf("\n");

    Fault_LogFPCSR(ctx->fpcsr);
    rmonPrintf("\n");

    Fault_LogFReg(0, &ctx->fp0.f.f_even);
    Fault_LogFReg(2, &ctx->fp2.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(4, &ctx->fp4.f.f_even);
    Fault_LogFReg(6, &ctx->fp6.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(8, &ctx->fp8.f.f_even);
    Fault_LogFReg(10, &ctx->fp10.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(12, &ctx->fp12.f.f_even);
    Fault_LogFReg(14, &ctx->fp14.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(16, &ctx->fp16.f.f_even);
    Fault_LogFReg(18, &ctx->fp18.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(20, &ctx->fp20.f.f_even);
    Fault_LogFReg(22, &ctx->fp22.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(24, &ctx->fp24.f.f_even);
    Fault_LogFReg(26, &ctx->fp26.f.f_even);
    rmonPrintf("\n");
    Fault_LogFReg(28, &ctx->fp28.f.f_even);
    Fault_LogFReg(30, &ctx->fp30.f.f_even);
    rmonPrintf("\n");
}

/**
 * Iterates through the active thread queue for a user thread with either
 * the CPU break or Fault flag set.
 */
OSThread* Fault_FindFaultedThread(void) {
    OSThread* thread = __osGetActiveQueue();

    // OS_PRIORITY_THREADTAIL indicates the end of the thread queue
    while (thread->priority != OS_PRIORITY_THREADTAIL) {
        if (thread->priority > OS_PRIORITY_IDLE && thread->priority < OS_PRIORITY_APPMAX &&
            (thread->flags & (OS_FLAG_CPU_BREAK | OS_FLAG_FAULT))) {
            return thread;
        }
        thread = thread->tlnext;
    }
    return NULL;
}

void Fault_Wait5Seconds(void) {
    s32 pad;
    OSTime start = osGetTime();

    do {
        Fault_Sleep(1000 / 60);
    } while ((osGetTime() - start) < OS_SEC_TO_CYCLES(5) + 1);

    sFaultInstance->autoScroll = true;
}

/**
 * Waits for the following button combination to be entered before returning:
 *
 * (L & R & Z) + DPad-Up + C-Down + C-Up + DPad-Down + DPad-Left + C-Left + C-Right + DPad-Right + (B & A & START)
 */
void Fault_WaitForButtonCombo(void) {
    Input* input = &sFaultInstance->inputs[0];
    s32 state;
    u32 s1;
    u32 s2;
    u32 pressedBtn;
    u32 curBtn;

    if (1) {}
    if (1) {}

    rmonPrintf(
        VT_FGCOL(WHITE) "KeyWaitB (ＬＲＺ " VT_FGCOL(WHITE) "上" VT_FGCOL(YELLOW) "下 " VT_FGCOL(YELLOW) "上" VT_FGCOL(WHITE) "下 " VT_FGCOL(WHITE) "左" VT_FGCOL(
            YELLOW) "左 " VT_FGCOL(YELLOW) "右" VT_FGCOL(WHITE) "右 " VT_FGCOL(GREEN) "Ｂ" VT_FGCOL(BLUE) "Ａ" VT_FGCOL(RED) "START" VT_FGCOL(WHITE) ")" VT_RST
                                                                                                                                                     "\n");
    rmonPrintf(VT_FGCOL(WHITE) "KeyWaitB'(ＬＲ左" VT_FGCOL(YELLOW) "右 +" VT_FGCOL(RED) "START" VT_FGCOL(
        WHITE) ")" VT_RST "\n");

    FaultDrawer_SetForeColor(GPACK_RGBA5551(255, 255, 255, 1));
    FaultDrawer_SetBackColor(GPACK_RGBA5551(0, 0, 0, 1));

    state = 0;
    s1 = 0;
    s2 = 1;

    while (state != 11) {
        Fault_Sleep(1000 / 60);
        Fault_UpdatePadImpl();

        pressedBtn = input->press.button;
        curBtn = input->cur.button;

        if (curBtn == 0 && s1 == s2) {
            s1 = 0;
        } else if (pressedBtn != 0) {
            if (s1 == s2) {
                state = 0;
            }

            switch (state) {
                case 0:
                    if (curBtn == (BTN_Z | BTN_L | BTN_R) && pressedBtn == BTN_Z) {
                        state = s2;
                        s1 = s2;
                    }
                    break;
                case 1:
                    if (pressedBtn == BTN_DUP) {
                        state = 2;
                    } else {
                        state = 0;
                    }
                    break;
                case 2:
                    if (pressedBtn == BTN_CDOWN) {
                        state = 3;
                        s1 = s2;
                    } else {
                        state = 0;
                    }
                    break;
                case 3:
                    if (pressedBtn == BTN_CUP) {
                        state = 4;
                    } else {
                        state = 0;
                    }
                    break;
                case 4:
                    if (pressedBtn == BTN_DDOWN) {
                        state = 5;
                        s1 = s2;
                    } else {
                        state = 0;
                    }
                    break;
                case 5:
                    if (pressedBtn == BTN_DLEFT) {
                        state = 6;
                    } else {
                        state = 0;
                    }
                    break;
                case 6:
                    if (pressedBtn == BTN_CLEFT) {
                        state = 7;
                        s1 = s2;
                    } else {
                        state = 0;
                    }
                    break;
                case 7:
                    if (pressedBtn == BTN_CRIGHT) {
                        state = 8;
                    } else {
                        state = 0;
                    }
                    break;
                case 8:
                    if (pressedBtn == BTN_DRIGHT) {
                        state = 9;
                        s1 = s2;
                    } else {
                        state = 0;
                    }
                    break;
                case 9:
                    if (pressedBtn == (BTN_A | BTN_B)) {
                        state = 10;
                    } else if (pressedBtn == BTN_A) {
                        state = 0x5B;
                    } else if (pressedBtn == BTN_B) {
                        state = 0x5C;
                    } else {
                        state = 0;
                    }
                    break;
                case 0x5B:
                    if (pressedBtn == BTN_B) {
                        state = 10;
                    } else {
                        state = 0;
                    }
                    break;
                case 0x5C:
                    if (pressedBtn == BTN_A) {
                        state = 10;
                    } else {
                        state = 0;
                    }
                    break;
                case 10:
                    if (pressedBtn == BTN_START) {
                        state = 11;
                    } else {
                        state = 0;
                    }
                    break;
            }
        }

        osWritebackDCacheAll();
    }
}

void Fault_DrawDisassemblyContents(u32 addr, u32 crashPc) {
    u32 alignedAddr = addr & ~3;
    u32 alignedCrashPc = crashPc & ~3;

    if (alignedAddr < K0BASE) {
        alignedAddr = K0BASE;
    }
    if (alignedAddr > (PHYS_TO_K0(0x800000) - 0x100)) {
        alignedAddr = (PHYS_TO_K0(0x800000) - 0x100);
    }

    Fault_FillScreenBlack();
    FaultDrawer_SetCharPad(-2, 0);

    FaultDrawer_DrawText(36, 18, "Disassembly at %08X", alignedAddr);
    if (alignedAddr >= K0BASE && alignedAddr < K2BASE) {
        u32* addr;
        u32 vAddr;
        s32 y;

        for (addr = (u32*)alignedAddr, y = 28; y != 226; y += 9, addr++) {
            char insn[40];

            MipsDecodeCPUInsn(insn, sizeof(insn), *addr, (u32)addr);

            if (addr == (u32*)crashPc) {
                FaultDrawer_SetFontColor(GPACK_RGBA5551(255, 0, 0, 1));
            }

            vAddr = Fault_ConvertAddress((uintptr_t)addr);
            if (vAddr == 0) {
                vAddr = (u32)addr;
            }

            FaultDrawer_DrawText(24, y, "%06X/%06X: %s", K0_TO_PHYS(addr), K0_TO_PHYS(vAddr), insn);

            if (addr == (u32*)crashPc) {
                FaultDrawer_SetFontColor(GPACK_RGBA5551(255, 255, 255, 255));
            }
        }
    }

    FaultDrawer_SetCharPad(0, 0);
}

s32 Fault_DrawDisassembly(u32 pc, u32 ra) {
    Input* input = &sFaultInstance->inputs[0];
    u32 addr = pc - 8;
    s32 count;
    u32 off;

    do {
        count = 0;
        if (addr < K0BASE) {
            addr = K0BASE;
        } else if (addr > PHYS_TO_K0(0x800000) - 4) {
            addr = PHYS_TO_K0(0x800000) - 4;
        }

        Fault_DrawDisassemblyContents(addr, pc);
        count = 600;

        while (sFaultInstance->autoScroll) {
            if (count == 0) {
                return false;
            }

            count--;
            Fault_Sleep(0x10);
            Fault_UpdatePadImpl();
            if (CHECK_BTN_ALL(input->press.button, BTN_L)) {
                sFaultInstance->autoScroll = false;
            }
        }

        do {
            Fault_Sleep(0x10);
            Fault_UpdatePadImpl();
        } while (input->press.button == 0);

        if (CHECK_BTN_ALL(input->cur.button, BTN_A)) {
            return false;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_START)) {
            return sRestartable;
        }

        off = 4; // jump 1 instruction
        if (CHECK_BTN_ALL(input->cur.button, BTN_Z)) {
            off = 4 * 4; // jump 4 instructions
        }
        if (CHECK_BTN_ALL(input->cur.button, BTN_B)) {
            off = 10 * 4; // jump 10 instructions
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DUP)) {
            addr -= off; // move up
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_DDOWN)) {
            addr += off; // move down
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CUP)) {
            addr = pc - 8; // go to crashing pc
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CLEFT)) {
            addr = ra - 8; // go to last ra
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CRIGHT)) {
            addr &= ~3; // go to branch target (or next instruction if not applicable)
            addr = MipsGetBranchTarget(*(u32*)addr, addr);
        }
    } while (!CHECK_BTN_ALL(input->press.button, BTN_L));

    sFaultInstance->autoScroll = true;
    return false;
}

void Fault_DrawMemDumpContents(const char* title, uintptr_t addr, u32 arg2) {
    uintptr_t alignedAddr = addr;
    u32* writeAddr;
    s32 y;
    s32 x;

    // Ensure address is within the bounds of RDRAM (Fault_DrawMemDump has already done this)
    if (alignedAddr < K0BASE) {
        alignedAddr = K0BASE;
    }
    // 8MB RAM, leave room to display 0x100 bytes on the final page
    //! @bug The loop below draws 22 * 4 * 4 = 0x160 bytes per page. Due to this, by scrolling further than
    //! 0x807FFEA0 some invalid bytes are read from outside of 8MB RDRAM space. This does not cause a crash,
    //! however the values it displays are meaningless. On N64 hardware these invalid addresses are read as 0.
    if (alignedAddr > K0BASE + 0x800000 - 0x100) {
        alignedAddr = K0BASE + 0x800000 - 0x100;
    }

    // Ensure address is word-aligned
    alignedAddr &= ~3;
    writeAddr = (u32*)alignedAddr;

    // Reset screen
    Fault_FillScreenBlack();
    FaultDrawer_SetCharPad(-2, 0);

    FaultDrawer_DrawText(36, 18, "%s %08x", title != NULL ? title : "PrintDump", alignedAddr);

    // Draw memory page contents
    if (alignedAddr >= K0BASE && alignedAddr < K2BASE) {
        for (y = 0; y < 22; y++) {
            FaultDrawer_DrawText(24, 28 + y * 9, "%06x", writeAddr);
            for (x = 0; x < 4; x++) {
                FaultDrawer_DrawText(82 + x * 52, 28 + y * 9, "%08x", *writeAddr++);
            }
        }
    }

    FaultDrawer_SetCharPad(0, 0);
}

/**
 * Draws the memory dump page.
 *
 * DPad-Up scrolls up.
 * DPad-Down scrolls down.
 * Holding Z while scrolling speeds up scrolling by a factor of 0x10.
 * Holding B while scrolling speeds up scrolling by a factor of 0x100.
 *
 * L toggles auto-scrolling pages.
 * START and A move on to the next page.
 *
 * @param pc Program counter, pressing C-Up jumps to this address
 * @param sp Stack pointer, pressing C-Down jumps to this address
 * @param cLeftJump Unused parameter, pressing C-Left jumps to this address
 * @param cRightJump Unused parameter, pressing C-Right jumps to this address
 */
s32 Fault_DrawMemDump(uintptr_t pc, uintptr_t sp, uintptr_t cLeftJump, uintptr_t cRightJump) {
    Input* input = &sFaultInstance->inputs[0];
    uintptr_t addr = pc;
    s32 scrollCountdown;
    u32 off;

    do {
        scrollCountdown = 0;
        // Ensure address is within the bounds of RDRAM
        if (addr < K0BASE) {
            addr = K0BASE;
        }
        // 8MB RAM, leave room to display 0x100 bytes on the final page
        if (addr > K0BASE + 0x800000 - 0x100) {
            addr = K0BASE + 0x800000 - 0x100;
        }

        // Align the address to 0x10 bytes and draw the page contents
        addr &= ~0xF;
        Fault_DrawMemDumpContents("Dump", addr, 0);
        scrollCountdown = 600;

        while (sFaultInstance->autoScroll) {
            // Count down until it's time to move on to the next page
            if (scrollCountdown == 0) {
                return false;
            }

            scrollCountdown--;
            Fault_Sleep(1000 / 60);
            Fault_UpdatePadImpl();
            if (CHECK_BTN_ALL(input->press.button, BTN_L)) {
                // Disable auto-scrolling
                sFaultInstance->autoScroll = false;
            }
        }

        // Wait for input
        do {
            Fault_Sleep(1000 / 60);
            Fault_UpdatePadImpl();
        } while (input->press.button == 0);

        // Move to next page
        if (CHECK_BTN_ALL(input->cur.button, BTN_A)) {
            return false;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_START)) {
            return sRestartable;
        }

        // Memory dump controls

        off = 0x10;
        if (CHECK_BTN_ALL(input->cur.button, BTN_Z)) {
            off *= 0x10;
        }

        if (CHECK_BTN_ALL(input->cur.button, BTN_B)) {
            off *= 0x100;
        }

        if (CHECK_BTN_ALL(input->press.button, BTN_DUP)) {
            addr -= off;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_DDOWN)) {
            addr += off;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CUP)) {
            addr = pc;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CDOWN)) {
            addr = sp;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CLEFT)) {
            addr = cLeftJump;
        }
        if (CHECK_BTN_ALL(input->press.button, BTN_CRIGHT)) {
            addr = cRightJump;
        }
    } while (!CHECK_BTN_ALL(input->press.button, BTN_L));

    // Resume auto-scroll and move to next page
    sFaultInstance->autoScroll = true;
    return false;
}

extern u8 _RomEnd[];

typedef struct {
    uintptr_t start;
    uintptr_t end;
    size_t nameOffset;
} SymInfo;

typedef struct {
    u32 numSyms;
    const SymInfo* syms;
    const char* strTab;
} FaultPDB;
#define PDB_MAX_NAME_LEN 60

static u32 sPrevPICfg[4];

static void Fault_PIGetAccess(void) {
    // Get exclusive access to the PI
    __osPiGetAccess();

    // Await any current DMA/IO completion
    u32 piStatus = IO_READ(PI_STATUS_REG);
    while (piStatus & (PI_STATUS_DMA_BUSY | PI_STATUS_IO_BUSY)) {
        piStatus = IO_READ(PI_STATUS_REG);
    }

    // Save previous Domain 1 configuration
    sPrevPICfg[0] = IO_READ(PI_BSD_DOM1_LAT_REG);
    sPrevPICfg[1] = IO_READ(PI_BSD_DOM1_PWD_REG);
    sPrevPICfg[2] = IO_READ(PI_BSD_DOM1_PGS_REG);
    sPrevPICfg[3] = IO_READ(PI_BSD_DOM1_RLS_REG);

    // Set cartridge access timings
    IO_WRITE(PI_BSD_DOM1_LAT_REG, gCartHandle->latency);
    IO_WRITE(PI_BSD_DOM1_PWD_REG, gCartHandle->pulse);
    IO_WRITE(PI_BSD_DOM1_PGS_REG, gCartHandle->pageSize);
    IO_WRITE(PI_BSD_DOM1_RLS_REG, gCartHandle->relDuration);
}

static void Fault_PIRelAccess(void) {
    // Restore previous Domain 1 configuration
    IO_WRITE(PI_BSD_DOM1_LAT_REG, sPrevPICfg[0]);
    IO_WRITE(PI_BSD_DOM1_PWD_REG, sPrevPICfg[1]);
    IO_WRITE(PI_BSD_DOM1_PGS_REG, sPrevPICfg[2]);
    IO_WRITE(PI_BSD_DOM1_RLS_REG, sPrevPICfg[3]);

    // Release PI
    __osPiRelAccess();
}

static s32 Fault_GetPDB(FaultPDB* out) {
    uintptr_t alignedRomEnd = (((uintptr_t)_RomEnd) + 0x100000-1) & ~(0x100000-1);
    uintptr_t pdbStart = PHYS_TO_K1(osRomBase | alignedRomEnd);

    // Lock PI
    Fault_PIGetAccess();

    // Read the magic number for the pdb to know if it's present
    u32 magic = *(u32*)(pdbStart + 0); // PIO
    if (magic != 0xFFFF5A5A) {
        // No pdb, no stack trace

        // Release the PI before printing
        Fault_PIRelAccess();
        rmonPrintf("[fault.c] NO PDB *(%08X)=%08X\n", pdbStart, magic);
        return false;
    }

    out->numSyms = *(u32*)(pdbStart + 4); // PIO
    out->syms = (SymInfo*)(pdbStart + 8);
    out->strTab = (const char*)(pdbStart + 8 + out->numSyms * sizeof(SymInfo));

    // Release PI
    Fault_PIRelAccess();
    return true;
}

#define IS_LW_RA_SP(insn)       ((insn##Hi) == 0x8FBF)  // lw $ra, <imm>($sp)
#define IS_ADDIU_SP_SP(insn)    ((insn##Hi) == 0x27BD)  // addiu $sp, $sp, <imm>
#define IS_SW_RA_SP(insn)       ((insn##Hi) == 0xAFBF)  // sw $ra, <imm>($sp)
#define IS_JR_RA(insn)          ((insn) == 0x03E00008)  // jr $ra

/**
 * Searches a single function's stack frame for the function it was called from.
 *
 * Since we use GCC and would prefer to debug with optimizations on to better reflect the state at release, there are
 * some codegen patterns involving reordered basic blocks that break heuristics that try to backtrace from current PC.
 *
 * This method of backtracing requires the presence of a "pdb" appended to the end of the ROM file after build,
 * containing:
 *  - A 32-bit magic number identifier. 0xFFFF5A5A was chosen as it cannot be confused for a PI open bus read.
 *  - A 32-bit number of symbol entries.
 *  - An array of function symbol entries sorted by start address, containing:
 *     - 32-bit vaddr start
 *     - 32-bit vaddr end
 *     - 32-bit name offset
 *  - A blob of function name strings, aligned to 4 bytes.
 *
 * To find the frame of the previous function, we first look up the current function in the pdb to find the function
 * bounds (and also record the name to display in the stack trace). Afterwards, we start at the beginning of the
 * function and work downwards searching for stack modifications, return address load/store and returns. Once everything
 * we need has been found we can move on to the next function.
 */
s32 Fault_WalkStack(FaultPDB* restrict pdb, char* nameOut, uintptr_t* spPtr, uintptr_t* pcPtr, uintptr_t* raPtr,
                    uintptr_t vpc) {
    uintptr_t sp = *spPtr;
    uintptr_t pc = *pcPtr;
    uintptr_t ra = *raPtr;

    // Lock PI
    Fault_PIGetAccess();

    // Find our function in the pdb using vpc.
    // The pdb entries are sorted and non-overlapping so we can binary search it.

    size_t low = 0;
    size_t high = pdb->numSyms - 1;

    if (pdb->syms[low].start > vpc || pdb->syms[high].end <= vpc) { // PIO
        // vpc is not in the pdb, abort
        Fault_PIRelAccess();
        rmonPrintf("VPC OUT OF BOUNDS FAIL\n");
        return false;
    }

    uintptr_t start;
    uintptr_t end;
    uintptr_t nameOffset = 0xFFFFFFFF;

    while (low <= high) {
        size_t mid = low + (high - low) / 2;

        const SymInfo* entry = &pdb->syms[mid];
        start = entry->start; // PIO
        end = entry->end; // PIO

        if (vpc >= start && vpc < end) {
            nameOffset = entry->nameOffset; // PIO
            break;
        } else if (vpc >= end) {
            low = mid + 1;
        } else { // if (vrom < entry->vromStart)
            high = mid - 1;
        }
    }
    if (nameOffset == 0xFFFFFFFF) {
        // failed to find entry
        Fault_PIRelAccess();
        rmonPrintf("PDB BSEARCH FAIL\n");
        return false;
    }

    // We found an entry, read the function name for later

    // Need to do 32-bit reads for PIO to work properly. Make sure gcc uses lw by using a volatile pointer.
    const volatile u32* nameData = (const volatile u32*)&pdb->strTab[nameOffset];

    char* nameCur = nameOut;
    size_t nameWords = 0;
    while (nameWords < PDB_MAX_NAME_LEN/sizeof(u32)) {
        u32 dat = nameData[nameWords++]; // PIO
        *nameCur++ = (dat >> 24) & 0xFF;
        *nameCur++ = (dat >> 16) & 0xFF;
        *nameCur++ = (dat >>  8) & 0xFF;
        *nameCur++ = (dat >>  0) & 0xFF;
        if ((dat & 0x000000FF) == 0) {
            // We aligned strings to 4 bytes in the pdb so we need only check the last
            // byte in a word to know when to stop reading.
            break;
        }
    }

    // No further need for PI access, release it.
    Fault_PIRelAccess();

    // rmonPrintf("[fault.c] FOUND PDB ENTRY [%08X %08X] : %08X %u \"%s\"\n",
    //            start, end,
    //            (uintptr_t)nameData, nameWords * 4, name);

    // Now that we have the function bounds and name we can search the stack frame for next pc/ra/sp

    u32 nextPC = pc;
    u32 nextRA = ra;
    u32 nextSP = sp;
    s32 gotRA = false;
    s32 gotSP = false;
    u32 lastInsn = 0x00000000;
    uintptr_t startRelocated = start + pc - vpc;
    uintptr_t endRelocated   = end + pc - vpc;
    uintptr_t tempPC;

    // rmonPrintf("BACKTRACE @ [pc=%08X sp=%08X ra=%08X] [%08X %08X]->[%08X %08X]\n",
    //            pc, sp, ra,
    //            start, end,
    //            startRelocated, endRelocated);

    // Has been executed already
    for (tempPC = startRelocated; tempPC < pc; tempPC += 4) {
        u32 insn = *(u32*)tempPC;
        u16 insnHi = insn >> 16;
        u16 insnLo = insn & 0xFFFF;
        s16 simm = insnLo;

        // char insnS[40];
        // MipsDecodeCPUInsn(insnS, sizeof(insnS), insn, tempPC);
        // rmonPrintf("    [%08X] %s\n", tempPC, insnS);

        if (IS_ADDIU_SP_SP(insn)) {
            // Stack pointer increment or decrement.
            // This has already executed so $sp has already been modified. Revert the modification
            nextSP = sp - simm;
            gotSP = true;
        } else if (IS_LW_RA_SP(insn)) {
            // Loading $ra from the stack.
            // This has already executed, meaning $ra is in the register already.
            nextPC = ra;
            gotRA = true;
        } else if (IS_SW_RA_SP(insn)) {
            // Saving $ra to the stack.
            // This has already executed, read $ra from the stack.
            nextPC = *(uintptr_t*)K0_TO_K1(sp + simm);
            gotRA = true;
        }

        // We ignore jr $ra if it comes before the PC so that we can descend into basic blocks reordered past the
        // nominal end of the function.

        if ((gotRA && gotSP)) {
            goto done;
        }
        lastInsn = insn;
    }

    // rmonPrintf("  ================\n");

    // Not yet executed
    for (tempPC = pc; tempPC < endRelocated; tempPC += 4) {
        u32 insn = *(u32*)tempPC;
        u16 insnHi = insn >> 16;
        u16 insnLo = insn & 0xFFFF;
        s16 simm = insnLo;

        // char insnS[40];
        // MipsDecodeCPUInsn(insnS, sizeof(insnS), insn, tempPC);
        // rmonPrintf("    [%08X] %s\n", tempPC, insnS);

        if (IS_ADDIU_SP_SP(insn)) {
            // Stack pointer increment or decrement.
            if (!gotSP) {
                // If we didn't already have SP we can use this to compute what SP would be upon return.
                nextSP = sp + simm;
            }
            gotSP = true;
        } else if (IS_LW_RA_SP(insn)) {
            // Loading $ra from the stack.
            // This has not yet executed, $ra is on the stack. Retrieve it if we don't already have it.
            if (!gotRA) {
                nextPC = *(uintptr_t*)K0_TO_K1(sp + simm);
            }
            gotRA = true;
        } else if (IS_SW_RA_SP(insn)) {
            // Saving $ra to the stack.
            // This has not yet executed, $ra is in the register. Retrieve it if we don't already have it.
            if (!gotRA) {
                nextPC = ra;
            }
            gotRA = true;
        }

        if (IS_JR_RA(lastInsn)) {
            // Found a jr $ra. Set PC to RA, we're done. (XXX do we have to set gotRA and wait for gotSP?)
            nextPC = ra;
            goto done;
        }

        if ((gotRA && gotSP)) {
            goto done;
        }
        lastInsn = insn;
    }

done:
    *pcPtr = nextPC;
    *raPtr = nextRA;
    *spPtr = nextSP;
    return true;
}

/**
 * Draws the stack trace page contents for the specified thread
 */
void Fault_DrawStackTrace(OSThread* thread, s32 x, s32 y, u32 height, u32 printfHeight) {
    char name[PDB_MAX_NAME_LEN];
    FaultPDB pdb;

    if (!Fault_GetPDB(&pdb)) {
        FaultDrawer_DrawText(x, y, "STACK TRACE UNAVAILABLE (NO PDB)");
        rmonPrintf("STACK TRACE UNAVAILABLE (NO PDB)\n");
        return;
    }

    uintptr_t sp = thread->context.sp;
    uintptr_t ra = thread->context.ra;
    uintptr_t pc = thread->context.pc;
    u32 maxHeight = MAX(height, printfHeight);

    FaultDrawer_DrawText(x, y, "SP       PC       (VPC)");
    rmonPrintf("STACK TRACE\nSP       PC       (VPC)\n");

    // Backtrace from the current function to the start of the thread
    for (u32 line = 1; line < maxHeight && (ra != 0 || sp != 0) && pc != (uintptr_t)__osCleanupThread; line++) {
        // Convert relocated address to virtual address if applicable
        uintptr_t vpc = Fault_ConvertAddress(pc);

        // Save current function pc/sp for printing later
        uintptr_t curPC = pc;
        uintptr_t curSP = sp;

        // Search current function for the previous function
        s32 success = Fault_WalkStack(&pdb, name, &sp, &pc, &ra, (vpc == 0) ? pc : vpc);
        if (!success) {
            rmonPrintf("ERROR in stack trace, aborting\n");
            break;
        }

        // Print info on current function, we do this after stack walking as we do not have the function name before
        if (line < height) {
            // TODO cram function name in somewhere?
            FaultDrawer_DrawText(x, y + line * 8, "%08x %08x", curSP, curPC);
            if (vpc != 0) {
                FaultDrawer_Printf(" -> %08x", vpc);
            }
        }
        if (line < printfHeight) {
            rmonPrintf("%08x %08x", curSP, curPC);
            if (vpc != 0) {
                rmonPrintf(" -> %08x %s\n", vpc, name);
            } else {
                rmonPrintf(" ->          %s\n", name);
            }
        }
    }
}

void Fault_ResumeThread(OSThread* thread) {
    thread->context.cause = 0;
    thread->context.fpcsr = FPCSR_FS | FPCSR_EV;
    thread->context.pc += 2*sizeof(u32);
    osStartThread(thread);
}

void Fault_DisplayFrameBuffer(void) {
    void* fb;

    osViSetYScale(1.0f);
    osViSetMode(&osViModeNtscLan1);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_DIVOT_OFF | OS_VI_DITHER_FILTER_ON);
    osViBlack(false);

    if (sFaultInstance->fb != NULL) {
        fb = sFaultInstance->fb;
    } else {
        fb = osViGetNextFramebuffer();
        if ((uintptr_t)fb == K0BASE) {
            fb = (void*)(PHYS_TO_K0(osMemSize) - sizeof(u16[SCREEN_HEIGHT][SCREEN_WIDTH]));
        }
    }

    osViSwapBuffer(fb);
    FaultDrawer_SetDrawerFB(fb, SCREEN_WIDTH, SCREEN_HEIGHT);
}

/**
 * Runs all registered fault clients. Each fault client displays a page
 * on the crash screen.
 */
s32 Fault_ProcessClients(void) {
    FaultClient* client;
    s32 idx = 0;

    for (client = sFaultInstance->clients; client != NULL; client = client->next) {
        if (client->callback == NULL) {
            continue;
        }
        Fault_FillScreenBlack();

        FaultDrawer_SetCharPad(-2, 0);
        FaultDrawer_Printf(FAULT_COLOR(DARK_GRAY) "CallBack (%d) %08x %08x %08x\n" FAULT_COLOR(WHITE), idx++,
                            client, client->arg0, client->arg1);
        FaultDrawer_SetCharPad(0, 0);

        if (client->callback(client->arg0, client->arg1)) {
            return true;
        }
        if (Fault_WaitForInput()) {
            return true;
        }

        Fault_DisplayFrameBuffer();
    }
    return false;
}

void Fault_UpdatePad(void) {
    Fault_UpdatePadImpl();
}

#define FAULT_MSG_CPU_BREAK ((OSMesg)1)
#define FAULT_MSG_FAULT ((OSMesg)2)
#define FAULT_MSG_UNK ((OSMesg)3)

s32 Fault_ThreadContext(void) {
    Fault_PrintThreadContext(sFaultInstance->faultedThread);
    Fault_LogThreadContext(sFaultInstance->faultedThread);
    return Fault_WaitForInput();
}

s32 Fault_StackTrace(void) {
    Fault_FillScreenBlack();
    FaultDrawer_DrawText(120, 16, "STACK TRACE");
    Fault_DrawStackTrace(sFaultInstance->faultedThread, 36, 24, 22, 50);
    return Fault_WaitForInput();
}

s32 Fault_Disassembly(void) {
    return Fault_DrawDisassembly(sFaultInstance->faultedThread->context.pc, sFaultInstance->faultedThread->context.ra);
}

s32 Fault_MemDump(void) {
    return Fault_DrawMemDump(sFaultInstance->faultedThread->context.pc - 0x100,
                             (uintptr_t)sFaultInstance->faultedThread->context.sp, 0, 0);
}

s32 Fault_EndPage(void) {
    Fault_FillScreenRed();
    FaultDrawer_DrawText(64, 80, "    CONGRATURATIONS!    ");
    FaultDrawer_DrawText(64, 90, "All Pages are displayed.");
    FaultDrawer_DrawText(64, 100, "       THANK YOU!       ");
    FaultDrawer_DrawText(64, 110, " You are great debugger!");
    return Fault_WaitForInput();
}

void Fault_ThreadEntry(void* arg) {
    OSMesg msg;
    OSThread* faultedThread;
    s32 pad;

    // Direct OS event messages to the fault event queue
    osSetEventMesg(OS_EVENT_CPU_BREAK, &sFaultInstance->queue, FAULT_MSG_CPU_BREAK);
    osSetEventMesg(OS_EVENT_FAULT, &sFaultInstance->queue, FAULT_MSG_FAULT);

    while (true) {
        do {
            // Wait for a thread to hit a fault
            osRecvMesg(&sFaultInstance->queue, &msg, OS_MESG_BLOCK);

            if (msg == FAULT_MSG_CPU_BREAK) {
                sFaultInstance->msgId = (u32)FAULT_MSG_CPU_BREAK;
                rmonPrintf("OS_EVENT_CPU_BREAK\n");
            } else if (msg == FAULT_MSG_FAULT) {
                sFaultInstance->msgId = (u32)FAULT_MSG_FAULT;
                rmonPrintf("OS_EVENT_FAULT\n");
            } else if (msg == FAULT_MSG_UNK) {
                Fault_UpdatePad();
                faultedThread = NULL;
                continue;
            } else {
                sFaultInstance->msgId = (u32)FAULT_MSG_UNK;
                rmonPrintf("[fault.c] UNKNOWN message! (%u)\n", (u32)msg);
            }

            faultedThread = __osGetCurrFaultedThread();
            rmonPrintf("__osGetCurrFaultedThread()=%08x\n", faultedThread);

            if (faultedThread == NULL) {
                faultedThread = Fault_FindFaultedThread();
                rmonPrintf("FindFaultedThread()=%08x\n", faultedThread);
            }
        } while (faultedThread == NULL);

        // Disable floating-point related exceptions
        __osSetFpcCsr(__osGetFpcCsr() & ~(FPCSR_EV | FPCSR_EZ | FPCSR_EO | FPCSR_EU | FPCSR_EI));
        sFaultInstance->faultedThread = faultedThread;
        sFaultInstance->exit = false;

        while (!sFaultInstance->faultHandlerEnabled) {
            Fault_Sleep(1000);
        }
        Fault_Sleep(1000 / 2);

        // Show fault framebuffer
        Fault_DisplayFrameBuffer();

        if (sFaultInstance->autoScroll) {
            Fault_Wait5Seconds();
        } else {
            // Draw error bar signifying the crash screen is available
            Fault_DrawCornerRec(GPACK_RGBA5551(255, 0, 0, 1));
        }

        // Set auto-scrolling and default colors
        sFaultInstance->autoScroll = true;
        FaultDrawer_SetForeColor(GPACK_RGBA5551(255, 255, 255, 1));
        FaultDrawer_SetBackColor(GPACK_RGBA5551(0, 0, 0, 0));

        size_t initPage = 0;
        if (sIsHungup) {
            // The rationale for this is you want to see the hungup message as the very first thing
            initPage = 2;
        }

        // Draw pages
        do {
            static s32 (*const handlers[])(void) = {
                Fault_ThreadContext,
                Fault_StackTrace,
                Fault_ProcessClients,
                Fault_Disassembly,
                Fault_MemDump,
                Fault_EndPage,
            };
            for (size_t i = initPage; i < ARRAY_COUNT(handlers); i++) {
                if (handlers[i]()) {
                    sFaultInstance->exit = true;
                    break;
                }
            }
            initPage = 0;
        } while (!sFaultInstance->exit);

        sRestartable = false;
        sIsHungup = false;
        Fault_ResumeThread(faultedThread);
    }
}

void Fault_SetFrameBuffer(void* fb, u16 w, u16 h) {
    sFaultInstance->fb = fb;
    FaultDrawer_SetDrawerFB(fb, w, h);
}

void Fault_Init(void) {
    sFaultInstance = &gFaultMgr;
    bzero(sFaultInstance, sizeof(FaultMgr));
    FaultDrawer_Init();
    FaultDrawer_SetInputCallback(Fault_WaitForInput);
    sFaultInstance->exit = false;
    sFaultInstance->msgId = 0;
    sFaultInstance->faultHandlerEnabled = false;
    sFaultInstance->faultedThread = NULL;
    sFaultInstance->padCallback = Fault_PadCallback;
    sFaultInstance->clients = NULL;
    sFaultInstance->autoScroll = false;
    gFaultMgr.faultHandlerEnabled = true;
    osCreateMesgQueue(&sFaultInstance->queue, &sFaultInstance->msg, 1);
    StackCheck_Init(&sFaultThreadInfo, sFaultStack, STACK_TOP(sFaultStack), 0, 0x100, "fault");
    osCreateThread(&sFaultInstance->thread, THREAD_ID_FAULT, Fault_ThreadEntry, NULL, STACK_TOP(sFaultStack),
                   THREAD_PRI_FAULT);
    osStartThread(&sFaultInstance->thread);
}

/**
 * Fault page for Hungup crashes. Displays the thread id and two messages
 * specified in arguments to `Fault_AddHungupAndCrashImpl`.
 */
s32 Fault_HungupFaultClient(void* arg0, void* arg1) {
    const char* exp1 = (const char*)arg0;
    const char* exp2 = (const char*)arg1;

    rmonPrintf("HungUp on Thread %d\n", osGetThreadId(NULL));
    rmonPrintf("%s\n", exp1 != NULL ? exp1 : "(NULL)");
    rmonPrintf("%s\n", exp2 != NULL ? exp2 : "(NULL)");
    FaultDrawer_Printf("HungUp on Thread %d\n", osGetThreadId(NULL));
    FaultDrawer_Printf("%s\n", exp1 != NULL ? exp1 : "(NULL)");
    FaultDrawer_Printf("%s\n", exp2 != NULL ? exp2 : "(NULL)");
    return false;
}

/**
 * Immediately crashes the current thread, for cases where an irrecoverable
 * error occurs. The parameters specify two messages detailing the error, one
 * or both may be NULL.
 */
NORETURN void Fault_AddHungupAndCrashImpl(const char* exp1, const char* exp2) {
    FaultClient client;
    s32 pad;

    Fault_AddClient(&client, Fault_HungupFaultClient, (void*)exp1, (void*)exp2);
    sIsHungup = true;
    *(u32*)0x11111111 = 0; // trigger an exception via unaligned memory access

    // Since the above line triggers an exception and transfers execution to the fault handler
    // this function does not return and the rest of the function is unreachable.
#ifdef __GNUC__
    __builtin_unreachable();
#endif
}

void Fault_AddHungupAndCrashRestartable(const char* exp1, const char* exp2) {
    FaultClient client;
    s32 pad;

    Fault_AddClient(&client, Fault_HungupFaultClient, (void*)exp1, (void*)exp2);
    sIsHungup = true;
    sRestartable = true;
    *(u32*)0x11111111 = 0; // trigger an exception via unaligned memory access

    // Unlike `Fault_AddHungupAndCrashImpl` this may return; make provisions for that:
    __asm__ volatile ("nop");       // gate the above out of the branch delay slot
    Fault_RemoveClient(&client);    // remove the hungup client
}

#undef Fault_AddHungupAndCrash
/**
 * Like `Fault_AddHungupAndCrashImpl`, however provides a fixed message containing
 * filename and line number
 */
NORETURN void Fault_AddHungupAndCrash(const char* file, int line) {
    char msg[256];

    sprintf(msg, "HungUp %s:%d", file, line);
    Fault_AddHungupAndCrashImpl(msg, NULL);
}
