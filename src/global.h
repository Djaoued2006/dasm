#ifndef GLOBAL_H
#define GLOBAL_H

char *instructions[] = {
    "push",
    "pop",
    "add",
    "sub",
    "mul",
    "div",
    "inc",
    "dec",
    "cmp",
    "jz",
    "jmp",
    "jg",
    "jl",
    "jle",
    "jge",
    "mov",
    "call",
    "ret",
    "syscall",
    "halt",
};

char *errors[] = {
    "NO_ERROR",
    "Invalid Token Type",
    "Expected More Tokens",
    "Expected a semi colon",
    "Expected a comma (for the second operand)",
};

char *registers[] = {
    "rax",
    "rbx",
    "rcx",
    "rdx",
};

char *token_kind[] = {
    "TT_NONE",
    "TT_INST",
    "TT_LABEL",
    "TT_REG",
    "TT_SEMI",
    "TT_COMMA",
    "TT_IMMEDIATE",
    "TT_LABEL_DEF",
};

#endif