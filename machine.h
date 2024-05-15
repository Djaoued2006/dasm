#ifndef MACHINE_H
#define MACHINE_H 

#include "./type.h"
#include <stdio.h>

INST get_curr_inst(Machine *dm) {
    return dm->prog[dm->ip];
}

INST_TYPE get_inst_type(Machine *dm) {
    return get_curr_inst(dm).type;
}

Word get_first_op(Machine *dm) {
    return get_curr_inst(dm).operand1;
}

Word get_sec_op(Machine *dm) {
    return get_curr_inst(dm).operand2;
}

Word *get_stack(Machine *dm) {
    return dm->stack;
}

size_t get_sp(Machine *dm) {
    return dm->sp;
}

size_t get_ip(Machine *dm) {
    return dm->ip;
}

Word *get_registers(Machine *dm) {
    return dm->registers;
}

Word get_reg_value(Machine *dm, REG reg) {
    return get_registers(dm)[reg];
}

void set_reg_value(Machine *dm, REG reg, Word value) {
    dm->registers[reg] = value;
}

void set_ip(Machine *dm, size_t ip) {
    dm->ip = ip;
}

void set_sp(Machine *dm, size_t sp) {
    dm->sp = sp;
}

void inc_ip(Machine *dm) {
    dm->ip++;
}



#endif