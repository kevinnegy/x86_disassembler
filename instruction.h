#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>
#define MAX_INSTR_LEN 15 
int DEFAULT_BIT_MODE;

// intel syntax has destination first like mips
// bytes should be stored as unsigned char, 2 digit hexadecimals

struct operands{
    int num_operands;
    char ** operands;
};

// Stores the byte instruction, the index of each segment, and the string assembly instruction
struct x86_instr{
    unsigned char * byte_code;

    // Store indices within byte code (maybe not necessary?)
    int prefix_ptr; 
    int rex_ptr;
    int opcode_ptr;
    int modrm_ptr;
    int sib_ptr;
    int displacement_ptr;
    int immediate_ptr; 

    unsigned char * prefixes; // 1 byte each 
    unsigned char rex; // 1 byte
    unsigned char * opcode; // 1,2,3 bytes
    unsigned char modrm; // 1 byte
    struct operands * operands; 
    unsigned char sib; // 1 byte
    unsigned char * displacement; // 0,1,2,4 bytes
    unsigned char * immediate; // 0,1,2,4 bytes
    int prefix_len;
    int opcode_len;
    int displacement_len;
    int immediate_len;
    
    char * x86_string;
};

void set_bit_mode(int mode);
struct x86_instr * create_x86_instr(char * bytes); 
char * get_string(struct x86_instr * inst); 
void disassemble_instr(struct x86_instr * inst);
void delete_x86_instr(struct x86_instr * inst);

#endif
