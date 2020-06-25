#ifndef IMMEDIATES_H
#define IMMEDIATES_H

#include "instruction.h"

unsigned long long get_displacement(struct x86_instr * inst, int mode);
unsigned long long get_immediate(struct x86_instr * inst, int mode);

#endif
