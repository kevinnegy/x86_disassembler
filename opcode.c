#include "opcode.h"
#include <stdio.h>
#include <string.h>
#include "prefix.h"
#include "registers.h"
#include "modrm.h"
#include "immediates.h"
void two_operand_op(struct x86_instr * inst, const char * op, int operand_order){
    int bit_mode = check_bit_mode(inst);

    if(inst->rex_ptr == -1)
        bit_mode = 32; //TODO check w bit to determine what mode/ handle 16 bit case

    inst->modrm_ptr = inst->opcode_ptr + 1;
    inst->modrm = inst->byte_code[inst->modrm_ptr];
    check_modrm(inst, bit_mode);

    strcat(inst->x86_string, op);
    strcat(inst->x86_string, inst->operands->operands[operand_order]);
    strcat(inst->x86_string, ", ");
    strcat(inst->x86_string, inst->operands->operands[(operand_order+1)%2]);
    
    return;
}

void check_third_byte_opcode(struct x86_instr * inst){;
}

void check_second_byte_opcode(struct x86_instr * inst){
    if(inst == NULL){
        printf("%s: %s\n", __func__, "instruction is null");
        return;
    }

    inst->opcode[1] = inst->byte_code[inst->opcode_ptr + 1];
    
    if(inst->opcode[1] == 0x38 || inst->opcode[1] == 0x3a){
        return check_third_byte_opcode(inst);
    }

    switch(inst->opcode[1]){
    case(op_jzje_84): //TODO are tttn encodings necessary?
        inst->displacement_ptr = inst->opcode_ptr + 2;
        long long disp = get_displacement(inst, DEFAULT_BIT_MODE);

        // relative displacement from next instruction; this instruction is 2 bytes + disp long;
        if(DEFAULT_BIT_MODE == 16)
            disp = disp + 4; //TODO test
        else
            disp = disp + 6; 

        sprintf(inst->displacement, "0x%llx", disp);

        strcat(inst->x86_string, "jz "); 
        strcat(inst->x86_string, inst->displacement);

        return;

    case(op_rdtsc):
        strcat(inst->x86_string, "rdtsc");
        return;

    default:
        printf("%s: %s %x\n", __func__, "second byte opcode invalid", inst->opcode[1]);
    }
    return;
}

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst){

    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->opcode[0]; 
    if(opcode == 0xf){
        return check_second_byte_opcode(inst);
    }

    switch(opcode){
    case(op_add_01):
        two_operand_op(inst, "add ", 1);
        return;
    case(op_call):
        inst->displacement_ptr = inst->opcode_ptr + 1;
        long long disp = get_displacement(inst, DEFAULT_BIT_MODE);

        // relative displacement from next instruction; this call instruction is 5 bytes long; next instruction 5 bytes from now
        disp = disp + 5; 
        sprintf(inst->displacement, "0x%llx", disp);

        strcat(inst->x86_string, "call ");
        strcat(inst->x86_string, inst->displacement);

        return;

    case(op_jmp_eb):
    {
        inst->displacement_ptr = inst->opcode_ptr + 1;
        unsigned int disp = get_displacement(inst, 8);
        disp = disp + 2; // jmp is 2 bytes
        sprintf(inst->displacement, "0x%x", disp);
        strcat(inst->x86_string, "jmp ");
        strcat(inst->x86_string, inst->displacement);
        return;
        
        
    }

    case(op_lea):
        two_operand_op(inst, "lea ", 0);
        return;

    case(op_mov):
        two_operand_op(inst, "mov ", 1);
        return;

    case(op_mov_8b):
        two_operand_op(inst, "mov ", 0);
        return;

    case(op_or):
        two_operand_op(inst, "or ", 1);
        return;

    case(op_shl):
    { 
        int bit_mode = check_bit_mode(inst);

        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];
        check_modrm(inst, bit_mode);

        inst->immediate_ptr = inst->modrm_ptr + 1;
        long long imm8 = get_immediate(inst, 8);
        sprintf(inst->immediate, "0x%llx", imm8);

        strcat(inst->x86_string, "shl ");
        strcat(inst->x86_string, inst->operands->operands[1]); 
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->immediate);

        return;
    }
    case(op_sub):   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
    {
        int bit_mode = check_bit_mode(inst);

        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];
        check_modrm(inst, bit_mode);
        
        inst->immediate_ptr = inst->modrm_ptr + 1;
        inst->immediate[0] = inst->byte_code[inst->immediate_ptr];
        unsigned char x = inst->immediate[0]; // TODO is it always immediate 8?
        char imm[5];
        sprintf(imm, "0x%x",x);

        strcat(inst->x86_string, "sub ");
        strcat(inst->x86_string, inst->operands->operands[1]); 
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, imm);
        
        return;
    }
    case(op_sub_2b):
        two_operand_op(inst, "sub ", 0);
        return;

    case(op_test_85):
        two_operand_op(inst, "test ", 1);
        return;
    }

    if(op_mov_b8 <= opcode && opcode < op_mov_b8 + 8){
        // Get register from opcode
        int bit_mode = check_bit_mode(inst);
        if(inst->rex_ptr == -1 || !(inst->rex & REX_W))
            bit_mode = 32; //TODO check w bit to determine what mode/ handle 16 bit case

        int index = opcode & 0x7;
        
        // Get extension
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?

        inst->immediate_ptr = inst->opcode_ptr + 1;
        unsigned long long imm = get_immediate(inst, bit_mode); // check bit mode 16, 32, 8
        sprintf(inst->immediate, "0x%llx", imm);

        strcat(inst->x86_string, "mov ");
        strcat(inst->x86_string, reg);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->immediate);
        return;
    } 
    else if(op_pop <= opcode &&  opcode < op_pop + 8){

        int bit_mode = check_bit_mode(inst);
        int index = opcode & 0x7;
        // Get extension
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?

        strcat(inst->x86_string, "pop ");
        strcat(inst->x86_string, reg);

        return;
    }
    else if(op_push <= opcode && opcode < op_push + 8){

        int bit_mode = check_bit_mode(inst);
        int index = opcode & 0x7;
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?
        
        strcat(inst->x86_string, "push ");
        strcat(inst->x86_string, reg);
        return;
    }
    switch(opcode){
    default:
        printf("%s: %s %x\n", __func__, "invalid opcode", opcode);
        return;
    }
    return;
}


void one_operand_op(struct x86_instr * inst){}
