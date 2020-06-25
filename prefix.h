#ifndef PREFIX_H
#define PREFIX_H

#include "instruction.h"

/** Prefixes **/
int find_opcode(struct x86_instr * inst);
int check_bit_mode(struct x86_instr * inst);

enum prefixes{
    prefix_lock = 0xf0,
    prefix_repn = 0xf2,
    prefix_rep = 0xf3,
    prefix_bnd = 0xf2, // TODO This is an iff prefix

    prefix_cs = 0x2e, 
    prefix_ss = 0x36, 
    prefix_ds = 0x3e, 
    prefix_es = 0x26, 
    prefix_fs = 0x64, 
    prefix_gs = 0x65, 
    prefix_branchn = 0x2e, // TODO another weird repeat
    prefix_branch = 0x3e, // TODO another weird repeat

    prefix_op_size_override = 0x66,
    prefix_addr_size_override = 0x67,
    
    prefix_rex = 0x40, 
};

//REX-prefix is for 64 bit mode instructions. Only used if instruction operates on 64bit registers. "If rex used when no meaning, it is ignored" (is that a problem?)
// one rex prefix perinstruction, must go right before opcode or escapeopcode byte(0FH) (after any mandatory prefixes)
// one byte REX prefix: 0100WRXB:
#define REX_prefix  (1 << 6)    // 0100 required
#define REX_W       (1 << 3)    // 0 = operand size determined by cs.d, 1 = 64 bit operand size
#define REX_R       (1 << 2)    // extension of ModR/M reg field
#define REX_X       (1 << 1)    // extension of SIB index field
#define REX_B       (1 << 0)    // extension of ModR/M r/m field, SIB base field, or Opcode reg field ( how is this determined?, seems like based on if r or x are set.)

int get_rex_w(struct x86_instr * inst);
int get_rex_r(struct x86_instr * inst);
int get_rex_x(struct x86_instr * inst);
int get_rex_b(struct x86_instr * inst);
#endif
