#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "./type.h"
#include "./prog.h"
#include "./stack.h"
#include "./consts.h"
#include "./regs.h"
#include "./machine.h"

void execute_arithmetic_instruction(Machine *dm) {

    Word a = get_reg_value(dm, get_first_op(dm)), b = get_reg_value(dm, get_sec_op(dm));

    switch (get_inst_type(dm))
    {
        case INST_ADD:
            a += b;
            break;

        case INST_SUB:
            a -= b;
            break;

        case INST_MUL:
            a *= b;
            break;

        case INST_DIV: 
            a /= b;
            break;

        default:
            break;
    }

    set_reg_value(dm, get_first_op(dm), a);
}

void dump(Machine *dm) {
    if (dm->prog[dm->ip].operand1 == STACK) {
        dump_stack(dm);
    } else {
        dump_reg(dm);
    }
}

void execute_instruction(Machine *dm) {
    printf("CURRENT INSTRUCTION: %s\n", INSTRUCTIONS[get_inst_type(dm)]);
    printf("\n");

    switch(get_inst_type(dm)) {
        case INST_NOP:
            inc_ip(dm);
            break;

        case INST_PUSH: 
            push(dm, get_first_op(dm));
            inc_ip(dm);
            break;

        case INST_POP:
            pop(dm);
            inc_ip(dm);
            break;

        case INST_ADD:
        case INST_SUB:
        case INST_MUL:
        case INST_DIV:
            execute_arithmetic_instruction(dm);
            inc_ip(dm);
            break;

        case INST_JUMP:
            set_ip(dm, get_first_op(dm));
            break;

        case INST_MOVE:
            move(dm);
            inc_ip(dm);
            break;

        case INST_DUMP:
            dump(dm);
            inc_ip(dm);
            break;

        case INST_COUNT:
            break;

        default:
            break;
    }
    return;
}

void execute(Machine *dm) {
    size_t count_number_instructions = 0;
    while (get_ip(dm) <  ARRAY_SIZE(prog)) {
        if (count_number_instructions == PROG_LIMIT_SIZE) 
            break;
        count_number_instructions++;
        execute_instruction(dm);
    }
}

void load_program_from_memory(Machine *dm, INST *prog, size_t prog_size) {
    memcpy(dm->prog, prog, prog_size * sizeof(prog[0]));
} 

void machine_init(Machine *dm) {
    set_sp(dm, 0);
    set_ip(dm, 0);

    for (size_t i = 0; i < R_COUNT; i++) 
        set_reg_value(dm, i, 0);

    load_program_from_memory(dm, prog, ARRAY_SIZE(prog));
}

int main(void) {
    machine_init(&dm);
    execute(&dm);
    return 0;
}