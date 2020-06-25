#ifndef MODRM_H
#define MODRM_H

#include "instruction.h"

void check_modrm(struct x86_instr * inst, int mode);

/** ModR/M **/
// ModR/M byte - addressing mode byte:
    // mod first two bits - something
    // reg/opcode 3bits - a register or further opcode
    // r/m 3 bits - another register or further opcode

// some ModR/M bytes require a second byte SIB 
// Scaled index byte:
    // scale
    // index - index register
    // base - base register


#endif
