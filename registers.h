#ifndef REGISTERS_H
#define REGISTERS_H
#include <stdio.h>

// Registers are stored in 3 bits

// word registers w = 1 (or default, no w bit needed)
enum regs_16{
    regs_16_ax = 0x0,
    regs_16_cx = 0x1,
    regs_16_dx = 0x2,
    regs_16_bx = 0x3,
    regs_16_sp = 0x4,
    regs_16_bp = 0x5,
    regs_16_si = 0x6,
    regs_16_di = 0x7,
    regs_16_r8w = 0x8,
    regs_16_r9w = 0x9,
    regs_16_r10w = 0xa,
    regs_16_r11w = 0xb,
    regs_16_r12w = 0xc,
    regs_16_r13w = 0xd,
    regs_16_r14w = 0xe,
    regs_16_r15w = 0xf,
};

enum regs_32{
    regs_32_eax = 0x0,
    regs_32_ecx = 0x1,
    regs_32_edx = 0x2,
    regs_32_ebx = 0x3,
    regs_32_esp = 0x4,
    regs_32_ebp = 0x5,
    regs_32_esi = 0x6,
    regs_32_edi = 0x7,
    regs_32_r8d = 0x8,
    regs_32_r9d = 0x9,
    regs_32_r10d = 0xa,
    regs_32_r11d = 0xb,
    regs_32_r12d = 0xc,
    regs_32_r13d = 0xd,
    regs_32_r14d = 0xe,
    regs_32_r15d = 0xf,
    
};

enum regs_64{
    regs_64_rax = 0x0,
    regs_64_rcx = 0x1,
    regs_64_rdx = 0x2,
    regs_64_rbx = 0x3,
    regs_64_rsp = 0x4,
    regs_64_rbp = 0x5,
    regs_64_rsi = 0x6,
    regs_64_rdi = 0x7,
    regs_64_r8  = 0x8,
    regs_64_r9  = 0x9,
    regs_64_r10 = 0xa,
    regs_64_r11 = 0xb,
    regs_64_r12 = 0xc,
    regs_64_r13 = 0xd,
    regs_64_r14 = 0xe,
    regs_64_r15 = 0xf,
    
};

// byte registers, w = 0 (required)
enum byte_regs{
    byte_regs_al = 0x0,
    byte_regs_cl = 0x1,
    byte_regs_dl = 0x2,
    byte_regs_bl = 0x3,
    byte_regs_ah = 0x4,
    byte_regs_ch = 0x5,
    byte_regs_dh = 0x6,
    byte_regs_bh = 0x7,
    byte_regs_r8l = 0x8,
    byte_regs_r9l = 0x9,
    byte_regs_r10l = 0xa,
    byte_regs_r11l = 0xb,
    byte_regs_r12l = 0xc,
    byte_regs_r13l = 0xd,
    byte_regs_r14l = 0xe,
    byte_regs_r15l = 0xf,
};


char * get_register(int index, int w, int mode);

#endif 

