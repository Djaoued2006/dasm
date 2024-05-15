#ifndef REGS_H
#define REGS_H

#include "./type.h"

void move(Machine *dm) {
    // the instruction will be: MOV reg, immediate
    dm->registers[dm->prog[dm->ip].operand1] =  dm->prog[dm->ip].operand2;
    return;
}


void dump_reg(Machine *dm) {
    printf("REGISTERS: \n");
    for (size_t i = 0; i < R_COUNT; i++) {
        printf("\t%s = %d\n", REGISTERS_NAME[i], dm->registers[i]);
    }
    printf("\n");
}


#endif