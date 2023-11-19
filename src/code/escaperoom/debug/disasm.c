#include "global.h"
#include "snprintf.h"

/**
 * MIPS Disassembler
 */

// clang-format off
static const char* const sO32GprNames[] = {
    "zero",
    "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9",
    "k0", "k1",
    "gp",
    "sp",
    "fp",
    "ra",
};

// https://sourceware.org/legacy-ml/binutils/2003-06/msg00436.html
static const char* const sEABI32GprNames[] = {
    "zero",
    "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9",
    "k0", "k1",
    "gp",
    "sp",
    "fp",
    "ra",
};

static const char* const sN32GprNames[] = {
    "zero",
    "at",
    "v0", "v1",
    "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
    "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9",
    "k0", "k1",
    "gp",
    "sp",
    "fp",
    "ra",
};

static const char* const sRSPGprNames[] = {
    "zero",
     "1",  "2",  "3",  "4",  "5",  "6",
     "7",  "8",  "9", "10", "11", "12",
    "13", "14", "15", "16", "17", "18",
    "19", "20", "21", "22", "23", "24",
    "25", "26", "27", "28", "29", "30",
    "ra",
};

static const char* const sVR4300Cop0Names[] = {
    "Index"     , "Random"    , "EntryLo0"  , "EntryLo1"  ,
    "Context"   , "PageMask"  , "Wired"     , "Reserved07",
    "BadVaddr"  , "Count"     , "EntryHi"   , "Compare"   ,
    "Status"    , "Cause"     , "EPC"       , "PRevID"    ,
    "Config"    , "LLAddr"    , "WatchLo"   , "WatchHi"   ,
    "XContext"  , "Reserved21", "Reserved22", "Reserved23",
    "Reserved24", "Reserved25", "PErr"      , "CacheErr"  ,
    "TagLo"     , "TagHi"     , "ErrorEPC"  , "Reserved31",
};

static const char* const sRSPCop0Names[] = {
    "SP_MEM_ADDR", "SP_DRAM_ADDR", "SP_RD_LEN"   , "SP_WR_LEN"   ,
    "SP_STATUS"  , "SP_DMA_FULL" , "SP_DMA_BUSY" , "SP_SEMAPHORE",
    "DPC_START"  , "DPC_END"     , "DPC_CURRENT" , "DPC_STATUS"  ,
    "DPC_CLOCK"  , "DPC_BUFBUSY" , "DPC_PIPEBUSY", "DPC_TMEM"    ,
};

/*
$fv0, $fv0f, $fv1, $fv1f
$ft0, $ft0f, $ft1, $ft1f, $ft2, $ft2f, $ft3, $ft3f,
$fa0, $fa0f, $fa1, $fa1f, $ft4, $ft4f, $ft5, $ft5f
$fs0, $fs0f, $fs1, $fs11, $fs2, $fs2f, $fs3, $fs3f, $fs4, $fs4f, $fs5, $fs5f
*/
static const char* const sO32Cop1Names[] = {
     "f0",  "f1",  "f2",  "f3",
     "f4",  "f5",  "f6",  "f7",  "f8",  "f9", "f10", "f11",
    "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19",
    "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
};

// TODO which are caller-saved?
// f0 is return
// f12-f19 are parameters
// 
static const char* const sEABI32Cop1Names[] = {
     "f0",  "f1",  "f2",  "f3",
     "f4",  "f5",  "f6",  "f7",  "f8",  "f9", "f10", "f11",
    "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19",
    "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
};

/*
$f0, $f2                Floating point function results     Caller-saved
$f1, $f3                Floating point temporaries          Caller-saved
$f4..$f11               Floating point temporaries          Caller-saved
$f12..$f19              Floating point arguments            Caller-saved
$f20..$f30 even (n32)   Floating point temporaries          Callee-saved
$f20..$f30 odd (n32)    Floating point                      Caller-saved
$f31                    Floating point status
*/
static const char* const sN32Cop1Names[] = {
     "f0",  "f1",  "f2",  "f3",
     "f4",  "f5",  "f6",  "f7",  "f8",  "f9", "f10", "f11",
    "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19",
    "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31",
};

static const char* const sVR4300Cop1CtrlNames[] = {
    [0] = "FCR0",
    [31] = "FpcCsr",
};

static const char* const sRSPCop2Names[] = {
     "v0",  "v1",  "v2",  "v3",  "v4",  "v5",  "v6",  "v7",
     "v8",  "v9", "v10", "v11", "v12", "v13", "v14", "v15",
    "v16", "v17", "v18", "v19", "v20", "v21", "v22", "v23",
    "v24", "v25", "v26", "v27", "v28", "v29", "v30", "v31",
};

static const char* const sRSPCop2CtrlNames[] = {
    "vco", "vcc", "vce"
};

typedef enum {
    MIPS_VR4300 = (1 << 0),
    MIPS_RSP    = (1 << 1)
} MipsMachine;

typedef struct {
    MipsMachine machine;
    const char* const * gprNames;
    size_t nGprNames;
    const char* const * cop0Names;
    size_t nCop0Names;
    const char* const * cop1Names;
    size_t nCop1Names;
    const char* const * cop1CtrlNames;
    size_t nCop1CtrlNames;
    const char* const * cop2Names;
    size_t nCop2Names;
    const char* const * cop2CtrlNames;
    size_t nCop2CtrlNames;
} MipsABI;

static const MipsABI sO32ABI = {
    MIPS_VR4300,
    // GPR
    sO32GprNames,           ARRAY_COUNT(sO32GprNames),
    // COP0
    sVR4300Cop0Names,       ARRAY_COUNT(sVR4300Cop0Names),
    // COP1
    sO32Cop1Names,          ARRAY_COUNT(sO32Cop1Names),
    sVR4300Cop1CtrlNames,   ARRAY_COUNT(sVR4300Cop1CtrlNames),
    // COP2
    NULL, 0,
    NULL, 0,
};

static const MipsABI sEABI32ABI = {
    MIPS_VR4300,
    // GPR
    sEABI32GprNames,        ARRAY_COUNT(sEABI32GprNames),
    // COP0
    sVR4300Cop0Names,       ARRAY_COUNT(sVR4300Cop0Names),
    // COP1
    sEABI32Cop1Names,       ARRAY_COUNT(sEABI32Cop1Names),
    sVR4300Cop1CtrlNames,   ARRAY_COUNT(sVR4300Cop1CtrlNames),
    // COP2
    NULL, 0,
    NULL, 0,
};

static const MipsABI sN32ABI = {
    MIPS_VR4300,
    // GPR
    sN32GprNames,           ARRAY_COUNT(sN32GprNames),
    // COP0
    sVR4300Cop0Names,       ARRAY_COUNT(sVR4300Cop0Names),
    // COP1
    sN32Cop1Names,          ARRAY_COUNT(sN32Cop1Names),
    sVR4300Cop1CtrlNames,   ARRAY_COUNT(sVR4300Cop1CtrlNames),
    // COP2
    NULL, 0,
    NULL, 0,
};

static const MipsABI sRSPABI = {
    MIPS_RSP,
    // GPR
    sRSPGprNames,       ARRAY_COUNT(sRSPGprNames),
    // COP0
    sRSPCop0Names,      ARRAY_COUNT(sRSPCop0Names),
    // COP1
    NULL, 0,
    NULL, 0,
    // COP2
    sRSPCop2Names,      ARRAY_COUNT(sRSPCop2Names),
    sRSPCop2CtrlNames,  ARRAY_COUNT(sRSPCop2CtrlNames),
};
// clang-format on

typedef enum {
    MIPS_ARGFMT_PASS = 0,     // empty arg
    MIPS_ARGFMT_CODE,         // break code
    MIPS_ARGFMT_CD,           // cop0 register d
    MIPS_ARGFMT_RD,           // general-purpose register d
    MIPS_ARGFMT_RS,           // general-purpose register s
    MIPS_ARGFMT_RT,           // general-purpose register t
    MIPS_ARGFMT_FD,           // floating-point register d
    MIPS_ARGFMT_FS,           // floating-point register s
    MIPS_ARGFMT_FT,           // floating-point register t
    MIPS_ARGFMT_SA,           // shift amount
    MIPS_ARGFMT_OP,           // cache op
    MIPS_ARGFMT_IMM,          // immediate value
    MIPS_ARGFMT_OFFSET_BASE,  // offset(base)
    MIPS_ARGFMT_OFFSET,       // branch offset
    MIPS_ARGFMT_TARGET,       // jump target
    MIPS_ARGFMT_VD,           // rsp vector register d
    MIPS_ARGFMT_VS,           // rsp vector register s
    MIPS_ARGFMT_VT,           // rsp vector register t
    MIPS_ARGFMT_VT_E,         // rsp vector register t indexed by e
    MIPS_ARGFMT_VT_ED,        // rsp vector register t indexed by ed
    MIPS_ARGFMT_VD_E,         // rsp vector register d indexed by e
    MIPS_ARGFMT_VD_ED,        // rsp vector register d indexed by ed
    MIPS_ARGFMT_VOFFSET_BASE, // rsp vector load/store offset(base)
    MIPS_ARGFMT_VC            // rsp vector control register
} MipsArg;

#define MIPS_INSN_MAX_ARGS 3
typedef struct {
    const char* mnemonic;
    u32 match;
    u32 mask;
    u8 args[MIPS_INSN_MAX_ARGS];
    u8 mul; // only used for vector load/stores
    u8 signExtend; // only used with instructions with ARGFMT_IMM or ARGFMT_OFFSET_BASE args
    u8 machines; // restrict to specific machine
} MipsOpInfo;

#define MIPS_ARGS(a, b, c) \
    { MIPS_ARGFMT_ ## a, MIPS_ARGFMT_ ## b, MIPS_ARGFMT_ ## c }

// clang-format off
const MipsOpInfo sMipsInsns[] = {
    // Pseudo-Instructions
    { "nop",        0x00000000, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "b",          0x10000000, 0xFFFF0000, MIPS_ARGS(OFFSET, PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "move",       0x00000025, 0xFC1F07FF, MIPS_ARGS(RD,     RS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "beqz",       0x10000000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bnez",       0x14000000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "negu",       0x00000023, 0xFFE007FF, MIPS_ARGS(RD,     RT,           PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "not",        0x00000027, 0xFC1F07FF, MIPS_ARGS(RD,     RS,           PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    // Instructions
    { "sll",        0x00000000, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "srl",        0x00000002, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "sra",        0x00000003, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "sllv",       0x00000004, 0xFC0007FF, MIPS_ARGS(RD,     RT,           RS    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "srlv",       0x00000006, 0xFC0007FF, MIPS_ARGS(RD,     RT,           RS    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "srav",       0x00000007, 0xFC0007FF, MIPS_ARGS(RD,     RT,           RS    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "jr",         0x00000008, 0xFC1FFFFF, MIPS_ARGS(RS,     PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "jalr",       0x0000F809, 0xFC1FFFFF, MIPS_ARGS(RS,     PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "syscall",    0x0000000C, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "break",      0x0000000D, 0xFC00FFFF, MIPS_ARGS(CODE,   PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "sync",       0x0000000F, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "mfhi",       0x00000010, 0xFFFF07FF, MIPS_ARGS(RD,     PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "mthi",       0x00000011, 0xFC1FFFFF, MIPS_ARGS(RS,     PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "mflo",       0x00000012, 0xFFFF07FF, MIPS_ARGS(RD,     PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "mtlo",       0x00000013, 0xFC1FFFFF, MIPS_ARGS(RS,     PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "dsllv",      0x00000014, 0xFC0007FF, MIPS_ARGS(RD,     RT,           RS    ), 0, 0,     MIPS_VR4300            },
    { "dsrlv",      0x00000016, 0xFC0007FF, MIPS_ARGS(RD,     RT,           RS    ), 0, 0,     MIPS_VR4300            },
    { "dsrav",      0x00000017, 0xFC0007FF, MIPS_ARGS(RD,     RT,           RS    ), 0, 0,     MIPS_VR4300            },
    { "mult",       0x00000018, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "multu",      0x00000019, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "div",        0x0000001A, 0xFC00FFFF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "divu",       0x0000001B, 0xFC00FFFF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "dmult",      0x0000001C, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "dmultu",     0x0000001D, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "ddiv",       0x0000001E, 0xFC00FFFF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "ddivu",      0x0000001F, 0xFC00FFFF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "add",        0x00000020, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "addu",       0x00000021, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "sub",        0x00000022, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "subu",       0x00000023, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "and",        0x00000024, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "or",         0x00000025, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "xor",        0x00000026, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "nor",        0x00000027, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "slt",        0x0000002A, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "sltu",       0x0000002B, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "dadd",       0x0000002C, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "daddu",      0x0000002D, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "dsub",       0x0000002E, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "dsubu",      0x0000002F, 0xFC0007FF, MIPS_ARGS(RD,     RS,           RT    ), 0, 0,     MIPS_VR4300            },
    { "tge",        0x00000030, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "tgeu",       0x00000031, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "tlt",        0x00000032, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "tltu",       0x00000033, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "teq",        0x00000034, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "tne",        0x00000036, 0xFC00FFFF, MIPS_ARGS(RS,     RT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "dsll",       0x00000038, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300            },
    { "dsrl",       0x0000003A, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300            },
    { "dsra",       0x0000003B, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300            },
    { "dsll32",     0x0000003C, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300            },
    { "dsrl32",     0x0000003E, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300            },
    { "dsra32",     0x0000003F, 0xFFE0003F, MIPS_ARGS(RD,     RT,           SA    ), 0, 0,     MIPS_VR4300            },
    { "bltz",       0x04000000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bgez",       0x04010000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bltzl",      0x04020000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300            },
    { "bgezl",      0x04030000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300            },
    { "tgei",       0x04080000, 0xFC1F0000, MIPS_ARGS(RS,     IMM,          PASS  ), 0, true,  MIPS_VR4300            },
    { "tgeiu",      0x04090000, 0xFC1F0000, MIPS_ARGS(RS,     IMM,          PASS  ), 0, false, MIPS_VR4300            },
    { "tlti",       0x040A0000, 0xFC1F0000, MIPS_ARGS(RS,     IMM,          PASS  ), 0, true,  MIPS_VR4300            },
    { "tltiu",      0x040B0000, 0xFC1F0000, MIPS_ARGS(RS,     IMM,          PASS  ), 0, false, MIPS_VR4300            },
    { "teqi",       0x040C0000, 0xFC1F0000, MIPS_ARGS(RS,     IMM,          PASS  ), 0, true,  MIPS_VR4300            },
    { "tnei",       0x040E0000, 0xFC1F0000, MIPS_ARGS(RS,     IMM,          PASS  ), 0, true,  MIPS_VR4300            },
    { "bltzal",     0x04100000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bgezal",     0x04110000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bltzall",    0x04120000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300            },
    { "bgezall",    0x04130000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300            },
    { "j",          0x08000000, 0xFC000000, MIPS_ARGS(TARGET, PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "jal",        0x0C000000, 0xFC000000, MIPS_ARGS(TARGET, PASS,         PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "beq",        0x10000000, 0xFC000000, MIPS_ARGS(RS,     RT,           OFFSET), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bne",        0x14000000, 0xFC000000, MIPS_ARGS(RS,     RT,           OFFSET), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "blez",       0x18000000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "bgtz",       0x1C000000, 0xFC1F0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "addi",       0x20000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "addiu",      0x24000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "slti",       0x28000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "sltiu",      0x2C000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, false, MIPS_VR4300 | MIPS_RSP },
    { "andi",       0x30000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, false, MIPS_VR4300 | MIPS_RSP },
    { "ori",        0x34000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, false, MIPS_VR4300 | MIPS_RSP },
    { "xori",       0x38000000, 0xFC000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, false, MIPS_VR4300 | MIPS_RSP },
    { "lui",        0x3C000000, 0xFFE00000, MIPS_ARGS(RT,     IMM,          PASS  ), 0, false, MIPS_VR4300 | MIPS_RSP },
    { "mfc0",       0x40000000, 0xFFE007FF, MIPS_ARGS(RT,     CD,           PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "mtc0",       0x40800000, 0xFFE007FF, MIPS_ARGS(RT,     CD,           PASS  ), 0, 0,     MIPS_VR4300 | MIPS_RSP },
    { "tlbr",       0x42000001, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "tlbwi",      0x42000002, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "tlbwr",      0x42000006, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "tlbp",       0x42000008, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "eret",       0x42000018, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "mfc1",       0x44000000, 0xFFE007FF, MIPS_ARGS(RT,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "dmfc1",      0x44200000, 0xFFE007FF, MIPS_ARGS(RT,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cfc1",       0x44400000, 0xFFE007FF, MIPS_ARGS(RT,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "mtc1",       0x44800000, 0xFFE007FF, MIPS_ARGS(RT,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "dmtc1",      0x44A00000, 0xFFE007FF, MIPS_ARGS(RT,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "ctc1",       0x44C00000, 0xFFE007FF, MIPS_ARGS(RT,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "bc1f",       0x45000000, 0xFFFF0000, MIPS_ARGS(OFFSET, PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "bc1t",       0x45010000, 0xFFFF0000, MIPS_ARGS(OFFSET, PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "bc1fl",      0x45020000, 0xFFFF0000, MIPS_ARGS(OFFSET, PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "bc1tl",      0x45030000, 0xFFFF0000, MIPS_ARGS(OFFSET, PASS,         PASS  ), 0, 0,     MIPS_VR4300            },
    { "add.s",      0x46000000, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "sub.s",      0x46000001, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "mul.s",      0x46000002, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "div.s",      0x46000003, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "sqrt.s",     0x46000004, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "abs.s",      0x46000005, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "mov.s",      0x46000006, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "neg.s",      0x46000007, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "round.l.s",  0x46000008, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "trunc.l.s",  0x46000009, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "ceil.l.s",   0x4600000A, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "floor.l.s",  0x4600000B, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "round.w.s",  0x4600000C, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "trunc.w.s",  0x4600000D, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "ceil.w.s",   0x4600000E, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "floor.w.s",  0x4600000F, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.d.s",    0x46000021, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.w.s",    0x46000024, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.l.s",    0x46000025, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.f.s",      0x46000030, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.un.s",     0x46000031, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.eq.s",     0x46000032, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ueq.s",    0x46000033, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.olt.s",    0x46000034, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ult.s",    0x46000035, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ole.s",    0x46000036, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ule.s",    0x46000037, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.sf.s",     0x46000038, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ngle.s",   0x46000039, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.seq.s",    0x4600003A, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ngl.s",    0x4600003B, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.lt.s",     0x4600003C, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.nge.s",    0x4600003D, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.le.s",     0x4600003E, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ngt.s",    0x4600003F, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "add.d",      0x46200000, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "sub.d",      0x46200001, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "mul.d",      0x46200002, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "div.d",      0x46200003, 0xFFE0003F, MIPS_ARGS(FD,     FS,           FT    ), 0, 0,     MIPS_VR4300            },
    { "sqrt.d",     0x46200004, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "abs.d",      0x46200005, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "mov.d",      0x46200006, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "neg.d",      0x46200007, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "round.l.d",  0x46200008, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "trunc.l.d",  0x46200009, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "ceil.l.d",   0x4620000A, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "floor.l.d",  0x4620000B, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "round.w.d",  0x4620000C, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "trunc.w.d",  0x4620000D, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "ceil.w.d",   0x4620000E, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "floor.w.d",  0x4620000F, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.s.d",    0x46200020, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.w.d",    0x46200024, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.l.d",    0x46200025, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.f.d",      0x46200030, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.un.d",     0x46200031, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.eq.d",     0x46200032, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ueq.d",    0x46200033, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.olt.d",    0x46200034, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ult.d",    0x46200035, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ole.d",    0x46200036, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ule.d",    0x46200037, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.sf.d",     0x46200038, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ngle.d",   0x46200039, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.seq.d",    0x4620003A, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ngl.d",    0x4620003B, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.lt.d",     0x4620003C, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.nge.d",    0x4620003D, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.le.d",     0x4620003E, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "c.ngt.d",    0x4620003F, 0xFFE007FF, MIPS_ARGS(FS,     FT,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.s.w",    0x46800020, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.d.w",    0x46800021, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.s.l",    0x46A00020, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "cvt.d.l",    0x46A00021, 0xFFFF003F, MIPS_ARGS(FD,     FS,           PASS  ), 0, 0,     MIPS_VR4300            },
    { "mfc2",       0x48000000, 0xFFE0007F, MIPS_ARGS(RT,     VD_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "mtc2",       0x48800000, 0xFFE0007F, MIPS_ARGS(RT,     VD_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "cfc2",       0x48400000, 0xFFE007FF, MIPS_ARGS(RT,     VC,           PASS  ), 0, 0,                   MIPS_RSP },
    { "ctc2",       0x48C00000, 0xFFE007FF, MIPS_ARGS(RT,     VC,           PASS  ), 0, 0,                   MIPS_RSP },
    { "vmulf",      0x4A000000, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmulu",      0x4A000001, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmudl",      0x4A000004, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmudm",      0x4A000005, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmudn",      0x4A000006, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmudh",      0x4A000007, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmacf",      0x4A000008, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmacu",      0x4A000009, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmadl",      0x4A00000C, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmadm",      0x4A00000D, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmadn",      0x4A00000E, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmadh",      0x4A00000F, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vadd",       0x4A000010, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vsub",       0x4A000011, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vabs",       0x4A000013, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vaddc",      0x4A000014, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vsubc",      0x4A000015, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vsar",       0x4A00001D, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vlt",        0x4A000020, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "veq",        0x4A000021, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vne",        0x4A000022, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vge",        0x4A000023, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vcl",        0x4A000024, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vch",        0x4A000025, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vcr",        0x4A000026, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vmrg",       0x4A000027, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vand",       0x4A000028, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vnand",      0x4A000029, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vor",        0x4A00002A, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vnor",       0x4A00002B, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vxor",       0x4A00002C, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vnxor",      0x4A00002D, 0xFE00003F, MIPS_ARGS(VD,     VS,           VT_E  ), 0, 0,                   MIPS_RSP },
    { "vrcp",       0x4A000030, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vrcpl",      0x4A000031, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vrcph",      0x4A000032, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vmov",       0x4A000033, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vrsq",       0x4A000034, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vrsql",      0x4A000035, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vrsqh",      0x4A000036, 0xFE00003F, MIPS_ARGS(VD_ED,  VT_E,         PASS  ), 0, 0,                   MIPS_RSP },
    { "vnop",       0x4A000037, 0xFFFFFFFF, MIPS_ARGS(PASS,   PASS,         PASS  ), 0, 0,                   MIPS_RSP },
    { "beql",       0x50000000, 0xfc000000, MIPS_ARGS(RS,     RT,           OFFSET), 0, 0,     MIPS_VR4300            },
    { "bnel",       0x54000000, 0xfc000000, MIPS_ARGS(RS,     RT,           OFFSET), 0, 0,     MIPS_VR4300            },
    { "blezl",      0x58000000, 0xfc1f0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300            },
    { "bgtzl",      0x5c000000, 0xfc1f0000, MIPS_ARGS(RS,     OFFSET,       PASS  ), 0, 0,     MIPS_VR4300            },
    { "daddi",      0x60000000, 0xfc000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, true,  MIPS_VR4300            },
    { "daddiu",     0x64000000, 0xfc000000, MIPS_ARGS(RT,     RS,           IMM   ), 0, true,  MIPS_VR4300            },
    { "ldl",        0x68000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "ldr",        0x6c000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "lb",         0x80000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "lh",         0x84000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "lwl",        0x88000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "lw",         0x8c000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "lbu",        0x90000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "lhu",        0x94000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "lwr",        0x98000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "lwu",        0x9c000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "sb",         0xa0000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "sh",         0xa4000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "swl",        0xa8000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "sw",         0xac000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300 | MIPS_RSP },
    { "sdl",        0xb0000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "sdr",        0xb4000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "swr",        0xb8000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "cache",      0xbc000000, 0xfc000000, MIPS_ARGS(OP,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "ll",         0xc0000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "lwc1",       0xc4000000, 0xfc000000, MIPS_ARGS(FT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "lld",        0xd0000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "ldc1",       0xd4000000, 0xfc000000, MIPS_ARGS(FT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "ld",         0xdc000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "sc",         0xe0000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "swc1",       0xe4000000, 0xfc000000, MIPS_ARGS(FT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "scd",        0xf0000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "sdc1",       0xf4000000, 0xfc000000, MIPS_ARGS(FT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "sd",         0xfc000000, 0xfc000000, MIPS_ARGS(RT,     OFFSET_BASE,  PASS  ), 0, true,  MIPS_VR4300            },
    { "lbv",        0xC8000000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 0, 0,                   MIPS_RSP },
    { "lsv",        0xC8000800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 1, 0,                   MIPS_RSP },
    { "llv",        0xC8001000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 2, 0,                   MIPS_RSP },
    { "ldv",        0xC8001800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 3, 0,                   MIPS_RSP },
    { "lqv",        0xC8002000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 4, 0,                   MIPS_RSP },
    { "lrv",        0xC8002800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 4, 0,                   MIPS_RSP },
    { "lpv",        0xC8003000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 3, 0,                   MIPS_RSP },
    { "luv",        0xC8003800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 3, 0,                   MIPS_RSP },
    { "ltv",        0xC8005800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 4, 0,                   MIPS_RSP },
    { "sbv",        0xE8000000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 0, 0,                   MIPS_RSP },
    { "ssv",        0xE8000800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 1, 0,                   MIPS_RSP },
    { "slv",        0xE8001000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 2, 0,                   MIPS_RSP },
    { "sdv",        0xE8001800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 3, 0,                   MIPS_RSP },
    { "sqv",        0xE8002000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 4, 0,                   MIPS_RSP },
    { "srv",        0xE8002800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 4, 0,                   MIPS_RSP },
    { "spv",        0xE8003000, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 3, 0,                   MIPS_RSP },
    { "suv",        0xE8003800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 3, 0,                   MIPS_RSP },
    { "stv",        0xE8005800, 0xFC00F800, MIPS_ARGS(VT_ED,  VOFFSET_BASE, PASS  ), 4, 0,                   MIPS_RSP },
};
// clang-format on

static const MipsABI* MipsGetABI(void) {
    const MipsABI* abi;
#if !defined(_MIPS_SIM)
    abi = &sEABI32ABI;
#elif (_MIPS_SIM == _ABIO32)
    abi = &sO32ABI;
#elif
    abi = &sN32ABI;
#else
# error "Unsupported ABI"
#endif
    return abi;
}

static const MipsOpInfo* MipsTableFindInsn(u32 data, const MipsABI* abi) {
    size_t i;

    for (i = 0; i < ARRAY_COUNT(sMipsInsns); i++) {
        const MipsOpInfo* op = &sMipsInsns[i];

        if ((data & op->mask) == op->match && (op->machines & abi->machine)) {
            return op;
        }
    }
    return NULL;
}

u32 MipsGetBranchTarget(u32 data, u32 addr) {
    const MipsOpInfo* op = MipsTableFindInsn(data, MipsGetABI());
    size_t i;

    for (i = 0; i < MIPS_INSN_MAX_ARGS && op->args[i] != MIPS_ARGFMT_PASS; i++) {
        switch (op->args[i]) {
            case MIPS_ARGFMT_TARGET:
                return (addr & 0xFC000000) | (_SHIFTR(data, 0, 26) << 2);
            case MIPS_ARGFMT_OFFSET:
                return addr + 4 + (s16)(_SHIFTR(data, 0, 16)) * 4;
        }
    }
    return addr + 4;
}

void MipsDecodeInsn(char* out, size_t outSize, u32 data, u32 vaddr, const MipsABI* abi) {
    const MipsOpInfo* match = MipsTableFindInsn(data, abi);
    s32 wrLen;
    size_t i;

    if (match == NULL) {
        // invalid/unknown instruction
        snprintf(out, outSize, "INVALID");
        return;
    }

    // got an instruction, emit mnemonic
    wrLen = snprintf(out, outSize, "%s ", match->mnemonic);
    outSize -= wrLen;
    out += wrLen;

    // emit args
    for (i = 0; i < MIPS_INSN_MAX_ARGS; i++) {
        s32 elemNo = -1;
        s32 regNo, imm;

        wrLen = 0;

        switch (match->args[i]) {
            case MIPS_ARGFMT_PASS:
                break;
            case MIPS_ARGFMT_CODE:
                wrLen = snprintf(out, outSize, "%d", (_SHIFTR(data, 6, 20) << 6) >> 16);
                break;
            case MIPS_ARGFMT_CD:
                regNo = _SHIFTR(data, 11, 5);
                wrLen = snprintf(out, outSize, "%s", abi->cop0Names[regNo]);
                break;
            case MIPS_ARGFMT_RD:
                regNo = _SHIFTR(data, 11, 5);
                goto emit_gpr;
            case MIPS_ARGFMT_RS:
                regNo = _SHIFTR(data, 21, 5);
                goto emit_gpr;
            case MIPS_ARGFMT_RT:
                regNo = _SHIFTR(data, 16, 5);
                goto emit_gpr;
emit_gpr:
                wrLen = snprintf(out, outSize, "%s", abi->gprNames[regNo]);
                break;
            case MIPS_ARGFMT_FD:
                regNo = _SHIFTR(data,  6,  5);
                goto emit_fpr;
            case MIPS_ARGFMT_FS:
                regNo = _SHIFTR(data, 11,  5);
                goto emit_fpr;
            case MIPS_ARGFMT_FT:
                regNo = _SHIFTR(data, 16,  5);
                goto emit_fpr;
emit_fpr:
                wrLen = snprintf(out, outSize, "%s", abi->cop1Names[regNo]);
                break;
            case MIPS_ARGFMT_SA:
                wrLen = snprintf(out, outSize, "%d", _SHIFTR(data, 6, 5));
                break;
            case MIPS_ARGFMT_OP:
                wrLen = snprintf(out, outSize, "0x%x", _SHIFTR(data, 16, 5));
                break;
            case MIPS_ARGFMT_OFFSET_BASE:
                regNo = _SHIFTR(data, 21,  5);
                imm = _SHIFTR(data, 0, 16);
                if (match->signExtend) {
                    imm = (s16)imm;
                }
                wrLen = snprintf(out, outSize, "%s0x%x(%s)", (imm < 0) ? "-" : "", ABS(imm), abi->gprNames[regNo]);
                break;
            case MIPS_ARGFMT_IMM:
                imm = _SHIFTR(data, 0, 16);
                if (match->signExtend) {
                    imm = (s16)imm;
                }
                wrLen = snprintf(out, outSize, "%s0x%x", (imm < 0) ? "-" : "", ABS(imm));
                break;
            case MIPS_ARGFMT_OFFSET:
                wrLen = snprintf(out, outSize, "0x%08X", vaddr + 4 + (s16)(_SHIFTR(data, 0, 16)) * 4);
                break;
            case MIPS_ARGFMT_TARGET:
                wrLen = snprintf(out, outSize, "0x%08X", (vaddr & 0xFC000000) | (_SHIFTR(data, 0, 26) << 2));
                break;
            case MIPS_ARGFMT_VD:
                regNo = _SHIFTR(data, 6, 5);
                goto emit_vreg;
            case MIPS_ARGFMT_VS:
                regNo = _SHIFTR(data, 11, 5);
                goto emit_vreg;
            case MIPS_ARGFMT_VT:
                regNo = _SHIFTR(data, 16, 5);
                goto emit_vreg;
            case MIPS_ARGFMT_VD_E:
                regNo = _SHIFTR(data, 6, 5);
                elemNo = _SHIFTR(data, 21, 4);
                goto emit_vreg;
            case MIPS_ARGFMT_VD_ED:
                regNo = _SHIFTR(data, 6, 5);
                elemNo = _SHIFTR(data, 7, 4);
                goto emit_vreg;
            case MIPS_ARGFMT_VT_E:
                regNo = _SHIFTR(data, 16, 5);
                elemNo = _SHIFTR(data, 21, 4);
                goto emit_vreg;
            case MIPS_ARGFMT_VT_ED:
                regNo = _SHIFTR(data, 16, 5);
                elemNo = _SHIFTR(data, 7, 4);
                goto emit_vreg;
emit_vreg:
                wrLen = snprintf(out, outSize, "%s", abi->cop2Names[regNo]);
                if (elemNo != -1) {
                    if (match->args[i] == MIPS_ARGFMT_VT_ED) {
                        /* This arg format only appears in vector load/stores, for which the
                           vector element specifier is the byte index rather than a slice */
                        outSize -= wrLen;
                        out += wrLen;
                        wrLen = snprintf(out, outSize, "[%d]", elemNo);
                    } else {
                        char* ch;
                        u32 mask;

                        if ((elemNo & 8) == 8) {
                            ch = "";
                            mask = 7;
                        } else if ((elemNo & 0xC) == 4) {
                            ch = "h";
                            mask = 3;
                        } else if ((elemNo & 0xE) == 2) {
                            ch = "q";
                            mask = 1;
                        } else {
                            // Don't emit [0]
                            break;
                        }
                        outSize -= wrLen;
                        out += wrLen;;
                        wrLen = snprintf(out, outSize, "[%d%s]", elemNo & mask, ch);
                    }
                }
                break;
            case MIPS_ARGFMT_VOFFSET_BASE:
                regNo = _SHIFTR(data, 21,  5);
                imm = _SHIFTR(data, 0, 7);
                if (imm & (1 << (7 - 1))) {
                    // sign extend
                    imm -= (1 << 7);
                }
                imm *= (1 << match->mul);
                wrLen = snprintf(out, outSize, "%s0x%x(%s)", (imm < 0) ? "-" : "", (unsigned)ABS(imm), abi->gprNames[regNo]);
                break;
            case MIPS_ARGFMT_VC:
                regNo = _SHIFTR(data, 11, 5);
                wrLen = snprintf(out, outSize, "%s", abi->cop2CtrlNames[regNo]); // TODO bounds check?
                break;
        }

        outSize -= wrLen;
        out += wrLen;

        if (i + 1 != MIPS_INSN_MAX_ARGS && match->args[i + 1] != MIPS_ARGFMT_PASS) {
            // Add comma if there is another arg following this one
            wrLen = snprintf(out, outSize, ", ");
            outSize -= wrLen;
            out += wrLen;
        }
    }
}

void MipsDecodeCPUInsn(char* out, size_t outSize, u32 data, u32 vaddr) {
    MipsDecodeInsn(out, outSize, data, vaddr, MipsGetABI());
}

void MipsDecodeRSPInsn(char* out, size_t outSize, u32 data, u32 vaddr) {
    MipsDecodeInsn(out, outSize, data, vaddr, &sRSPABI);
}
