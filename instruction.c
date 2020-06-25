#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"

void set_bit_mode(int mode){
    switch(mode){
        case(16):
            DEFAULT_BIT_MODE = 16;
            break;
        case(32):
            DEFAULT_BIT_MODE = 32;
            break;
        case(64):
            DEFAULT_BIT_MODE = 64;
            break;
        default:
            printf("%s: %s %d\n", __func__, "invalid bit mode", mode);
            printf("Setting default to 64\n");
            DEFAULT_BIT_MODE = 64;
    }
}

void build_x86_string(struct x86_instr * inst);
void check_sib(struct x86_instr * inst);
void check_displacement(struct x86_instr * inst);
void check_immediate(struct x86_instr * inst);

struct x86_instr * create_x86_instr(char * bytes){ 
    if(bytes == NULL){
         printf("%s: input bytes == NULL\n", __func__); exit(-1); 
    }

    struct x86_instr * inst = (struct x86_instr *) malloc(sizeof(struct x86_instr));
    if(inst == NULL){
        printf("%s: %s\n", __func__, "inst malloc failed");
    }
    
    // Instantiate all index ptrs to -1
    inst->prefix_ptr = inst->rex_ptr = inst->opcode_ptr = inst->modrm_ptr =  
        inst->sib_ptr = inst->immediate_ptr = inst->displacement_ptr = -1; 

    // Store original byte code
    inst->byte_code = (unsigned char *) malloc(sizeof(unsigned char) * MAX_INSTR_LEN); 
    if(inst->byte_code == NULL){
        printf("%s: %s\n", __func__, "inst->byte_code malloc failed");
    }
    memcpy(inst->byte_code, bytes, MAX_INSTR_LEN); 

    inst->rex = inst->modrm = inst->sib = 0x0;

    // Malloc max array lens for each instruction segment
    inst->prefixes = (unsigned char *) malloc(sizeof(unsigned char) * 4); // 4 groups, 1 each max
    if(inst->prefixes == NULL){
        printf("%s: %s\n", __func__, "inst->prefixes malloc failed");
    }
    inst->opcode = (unsigned char *) malloc(sizeof(unsigned char) * 3);  
    if(inst->opcode == NULL){
        printf("%s: %s\n", __func__, "inst->opcode malloc failed");
    }
    inst->displacement = (unsigned char *) malloc(sizeof(unsigned char) * 4);  
    if(inst->displacement == NULL){
        printf("%s: %s\n", __func__, "inst->displacement malloc failed");
    }
    inst->immediate = (unsigned char *) malloc(sizeof(unsigned char) * 11);  // 0x and 8 hex (10 total + '\0')
    if(inst->immediate == NULL){
        printf("%s: %s\n", __func__, "inst->immediate malloc failed");
    }


    inst->operands = (struct operands *) malloc(sizeof(struct operands));
    inst->operands->num_operands = 0;

    // Start off array lens at 0
    inst->prefix_len = inst->opcode_len = inst->displacement_len = inst->immediate_len = 0; 
    
    inst->x86_string = (char *) malloc(sizeof(char) * 50);
    if(inst->x86_string == NULL){
        printf("%s: %s\n", __func__, "inst->x86_string malloc failed");
    }

    return inst;

}

void delete_x86_instr(struct x86_instr * inst){
    free(inst->byte_code);
    free(inst->prefixes);
    free(inst->opcode);
    free(inst->displacement);
    if(inst->operands->num_operands > 0)
        free(inst->operands->operands); // Don't free strings in operands ** (see registers.h)
    free(inst->operands);
    free(inst->immediate);
    free(inst->x86_string);
    free(inst);

    return;
}

char * get_string(struct x86_instr * inst){
    return inst->x86_string;
}
