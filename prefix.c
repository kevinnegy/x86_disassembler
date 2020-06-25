#include "prefix.h"
#include <stdio.h>
#include <stdlib.h>

// Cycles through prefixes until opcode is found, returns opcode index
int find_opcode(struct x86_instr * inst){
    if(inst == NULL){
        printf("%s: %s\n", __func__, "inst == NULL");
        return -1;
    }
    int i = 0;
    int opcode_flag = 0; // turn on when prefix not found

    unsigned char * bytes = inst-> byte_code;
    if(bytes == NULL){
        printf("%s: %s\n", __func__, "byte_code is NULL");
        return -1; 
    }

    while(opcode_flag == 0 && i < MAX_INSTR_LEN){
        switch(bytes[i]){
        case(prefix_lock):
        case(prefix_repn): // Must also include bnd
        case(prefix_rep):
        case(prefix_cs): // Must include branchn
        case(prefix_ss):
        case(prefix_ds): // Must include branch
        case(prefix_es):
        case(prefix_fs):
        case(prefix_gs):
        case(prefix_op_size_override):
        case(prefix_addr_size_override):
            inst->prefix_ptr = 0; // Instruction contains prefix
            goto append;
        }

        // Check for REX prefix
        if(prefix_rex <= bytes[i] && bytes[i] <= prefix_rex + 0xf){
            inst->rex_ptr = i;
            inst->rex = bytes[i];
            i++; // REX must come before opcode
        }

        inst->opcode[0] = bytes[i];
        inst->opcode_ptr = i;
        opcode_flag = 1;
        goto out;
append: 
        printf("append needed");

out:
        if(opcode_flag == 1)
            break;
        i++;
    }

    return i;
}

// Returns 16, 32, 64
// TODO should this go in instruction.c?
int check_bit_mode(struct x86_instr * inst){
    if (inst->rex != 0x0 && inst->rex & REX_W){
        return 64;   
    }
    return DEFAULT_BIT_MODE;
}

int get_rex_w(struct x86_instr * inst){
    unsigned char rex = inst->rex;
    if(rex & REX_prefix != REX_prefix)
        return 0;
    return (rex & REX_W) >> 3;
}

int get_rex_r(struct x86_instr * inst){
    unsigned char rex = inst->rex;
    if(rex & REX_prefix != REX_prefix)
        return 0;
    return (rex & REX_R) >> 2;
}

int get_rex_x(struct x86_instr * inst){
    unsigned char rex = inst->rex;
    if(rex & REX_prefix != REX_prefix)
        return 0;
    return (rex & REX_X) >> 1;
}

int get_rex_b(struct x86_instr * inst){
    unsigned char rex = inst->rex;
    if(rex & REX_prefix != REX_prefix)
        return 0;
    return (rex & REX_B) >> 0;
}
// Group 1:
    // FO - LOCK prefix - exclusive use of shared memory in multiprocessor environ
    // F2 - REPNE/REPNZ (repeat not equal/zero) - repeat for each element of string or sometimes mandatory for instructions
    // F3 - REP/REPE/REPZ - repeat (equal/zero) - mandatory for POPCNT, LZCNT, ADOX
    // F2 - BND prefix iff:
            // CPUID.(EAX=07, ECX=0):EBX.MPX[bit 14] is set
            // BNDCFGU.EN and/or IA32_BNDCFGS.EN is set
            // When F2 precedes a near CALL, near RET, near JMP, short Jcc, or near Jcc instruction

// Group 2:
    // 2E - CS segment override prefix
    // 36 - SS segment override prefix
    // 3E - DS segment override prefix
    // 26 - ES segment override prefix
    // 64 - FS segment override prefix
    // 65 - GS segment override prefix

    // 2E - Branch not taken (only Jcc instructions)
    // 3E - Branch taken (only Jcc instructions)

// Group 3:
    // 66 - operand size override prefix (switch between 16 and 32 bit operands; use to select non-default) or mandatory

// Group 4:
    // 67 - address size override prefix (switch between 16 and 32 bit addressing)
