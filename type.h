#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>

#include "./consts.h"
#include "./macros.h"
typedef int Word;

typedef enum {
    STACK,
    REGISTERS,
} MEMORY_TYPE;


typedef enum {
    R_AX = 0,
    R_BX, 
    R_CX, 
    R_DX,
    R_SI, 
    R_DI, 
    R_COUNT,
} REG;

char *REGISTERS_NAME[R_COUNT] = {
    "R_AX",
    "R_BX", 
    "R_CX", 
    "R_DX",
    "R_SI",
    "R_DI",
};

typedef enum {
    INST_NOP = 0,
    INST_PUSH,
    INST_POP,
    INST_ADD, 
    INST_SUB,
    INST_MUL, 
    INST_DIV,
    INST_JUMP,
    INST_MOVE, 
    INST_DUMP,
    INST_COUNT,
} INST_TYPE;

char *INSTRUCTIONS[INST_COUNT] = 
{   "INST_NOP",
    "INST_PUSH",
    "INST_POP",
    "INST_ADD", 
    "INST_SUB",
    "INST_MUL",
    "INST_DIV",
    "INST_JUMP",
    "INST_MOVE",
    "INST_DUMP",
};

typedef enum {
    STACK_OVERFLOW_ERROR, 
    STACK_UNDERFLOW_ERROR,
    NO_ERROR,
} ERROR_TYPE;


typedef struct {
    INST_TYPE type;
    Word operand1, operand2;
} INST;

typedef struct {
    Word stack[STACK_SIZE];
    size_t sp;
    INST prog[PROG_SIZE];
    size_t ip;
    Word registers[R_COUNT];
} Machine;

Machine dm = {0};

#endif