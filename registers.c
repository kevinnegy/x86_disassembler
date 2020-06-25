#include "registers.h"
#include <stdio.h>

static char * get_register_16(int index){
    char * strings[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
    return strings[index];
}

static char * get_register_32(int index){
    char * strings[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
    return strings[index];
}

static char * get_register_64(int index){
    char * strings[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    return strings[index];
}


static char * get_byte_register(int index){
    char * strings[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "r8l", "r9l", "r10l", "r11l", "r12l", "r13l", "r14l", "r15l"};
    return strings[index];
}

char * get_register(int index, int w, int mode){
    if(index < 0 || index > 15){
        printf("%s: %s\n", __func__, "register out of range");
        return NULL;
    }

    if(w == 0 && mode != 64)
        return get_byte_register(index);
    else if(mode == 16)
        return get_register_16(index);
    else if(mode == 32)
        return get_register_32(index);
    else if(mode == 64)
        return get_register_64(index);
    return NULL;        
}
