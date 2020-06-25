#include <stdio.h>
#include "immediates.h"


static unsigned long long calc_displacement(struct x86_instr * inst, int num_bytes){
    int i = 0;
    unsigned long long disp = 0;
    for(i = 0; i< num_bytes; i++){
        unsigned char byte = inst->byte_code[inst->displacement_ptr + i];
        disp = disp + ((unsigned long long)byte << (i*8));  // little endian!
    } 
    return disp; 
}

unsigned long long get_displacement(struct x86_instr * inst, int mode){
    if(inst == NULL){
        printf("%s: %s\n", __func__, "instruction is null");
        return 0;
    }

    switch(mode){
        case(8):
            return calc_displacement(inst, 1);
        case(16): // 2 bytes to read
            return calc_displacement(inst, 2);
        case(32): // 4 bytes 
            return calc_displacement(inst, 4);
        case(64): // 32 bit processing with 64 bit sign extend
        {
            unsigned long long disp = calc_displacement(inst, 4);
            unsigned long long sign_32 = 0x1 & (disp >> 31);

            if(sign_32 == 0) // positive sign extend 
                disp = (sign_32 << 63) + (disp & 0x7fffffff);
            else // negative sign extend
                disp = 0xffffffff00000000 + disp;

            return disp;
        }
        default:
            printf("%s: %s %d\n", __func__, "invalid mode", mode);
    }
   return 0; 
}

static unsigned long long calc_immediate(struct x86_instr * inst, int num_bytes){
    int i = 0;
    unsigned long long disp = 0;
    for(i = 0; i< num_bytes; i++){
        unsigned char byte = inst->byte_code[inst->immediate_ptr + i];
        disp = disp + ((unsigned long long)byte << (i*8));  // little endian!
    } 
    return disp; 
}

unsigned long long get_immediate(struct x86_instr * inst, int mode){
    if(inst == NULL){
        printf("%s: %s\n", __func__, "instruction is null");
        return 0;
    }
    switch(mode){
        case(8): // 1 bytes to read
            return calc_immediate(inst, 1);
        case(16): // 2 bytes to read
            return calc_immediate(inst, 2);
        case(32): // 4 bytes 
            return calc_immediate(inst, 4);
        case(64): // 32 bit processing with 64 bit sign extend
            return calc_immediate(inst, 8);
        default:
            printf("%s: %s %d\n", __func__, "invalid mode", mode);
    }
   return 0; 

    
}
