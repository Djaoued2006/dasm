#ifndef STACK_H
#define STACK_H

#include "./type.h"
#include "./consts.h"

void dump_stack(Machine *dm) {
    printf("STACK:\n"); 
    for (size_t i = 0; i < dm->sp; i++) {
        printf("%10d\n", dm->stack[i]);
    }
    printf("\n");
}

void push(Machine *dm, Word value) {
    if (dm->sp == STACK_SIZE) {
        printf("ERROR: STACK OVERFLOW\n");
        // dump_stack(dm);
        exit(1);
    }
    dm->stack[dm->sp++] = value;
    return;
}

Word pop(Machine *dm) {
    if (dm->sp == 0) {
        printf("ERROR: STACK UNDERFLOW\n");
        // dump_stack(dm);
        exit(1);
    }
    dm->sp--;
    return dm->stack[dm->sp];
}

#endif