#ifndef DISASM_H
#define DISASM_H

#include "ultra64.h"

u32 MipsGetBranchTarget(u32 data, u32 addr);

void MipsDecodeCPUInsn(char* out, size_t outSize, u32 data, u32 vaddr);
void MipsDecodeRSPInsn(char* out, size_t outSize, u32 data, u32 vaddr);

#endif
