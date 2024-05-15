#ifndef MACROS_H
#define MACROS_H

#define ARRAY_SIZE(array) sizeof(array) / sizeof((array)[0])

#define MAKE_INST_NOP()                          { .type = INST_NOP }

#define MAKE_INST_PUSH(op)                       { .type = INST_PUSH, .operand1 = (op) }
#define MAKE_INST_POP()                          { .type = INST_POP }

#define MAKE_INST_ADD(op1, op2)                  { .type = INST_ADD, .operand1 = op1, .operand2 = op2}
#define MAKE_INST_SUB(op1, op2)                  { .type = INST_SUB, .operand1 = op1, .operand2 = op2}
#define MAKE_INST_MUL(op1, op2)                  { .type = INST_MUL, .operand1 = op1, .operand2 = op2}
#define MAKE_INST_DIV(op1, op2)                  { .type = INST_DIV, .operand1 = op1, .operand2 = op2}

#define MAKE_INST_JUMP(op)                       { .type = INST_JUMP, .operand1 = (op) }

#define MAKE_INST_MOVE(op1, op2)                 { .type = INST_MOVE, .operand1 = op1, .operand2 = op2 }

#define MAKE_INST_DUMP(op)                       { .type = INST_DUMP, .operand1 = op }

#endif