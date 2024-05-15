#ifndef PROG_H
#define PROG_H

#include "./macros.h"
#include "./type.h"

INST prog[] = {
    MAKE_INST_MOVE(R_AX, 2),
    MAKE_INST_MOVE(R_BX, 2),
    MAKE_INST_ADD(R_AX, R_BX),
    MAKE_INST_DUMP(REGISTERS),
    
    // MAKE_INST_JUMP(0),
};

#endif
