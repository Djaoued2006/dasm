#ifndef ASM_H
#define ASM_H

#include "./lexer.h"
#include "../dm/defs.h"
#include "../dm/init.h"

Program prog_init() {
    return (Program) {
        .prog_size = 0,
        .label_count = 0,
    };
}

int prog_full(Program *prog) {
    return (prog->prog_size == PROGRAM_CAPACITY);
}

void prog_push_inst(Program *prog, Inst inst) {
    if (prog_full(prog)) {
        fprintf(stderr, "ERROR: too much instructions\n");
        exit(1);
    }

    prog->instructions[prog->prog_size++] = inst; 
}

typedef enum {
    Parser_NO_ERROR,
    Parser_ERROR_INVALID_TOKEN_TYPE,
    Parser_ERROR_EXPECTED_MORE_TOKENS,
    Parser_ERROR_EXPECTED_SEMI_COLON,
    Parser_ERROR_EXPECTED_COMMA,
} Parser_Error_Type;


typedef struct {
    Parser_Error_Type err;

    Tokens tokens;
    Token_Node *current;
    
    Program prog;
} Parser;

Parser asm_init(Tokens tokens) {
    return (Parser) {
        .err = Parser_NO_ERROR,
        .tokens = tokens,
        .current = tokens.head,
        .prog = prog_init(),
    };
}

int expect_token(Parser *parser, Token_Kind kind) {
    Token_Node *next = parser->current;
    if (next == NULL) return 0;
    if (next->token.kind != kind) return 0;
    return 1; 
}

void consume_token(Parser *parser) {
    parser->current = parser->current->next;
}

void parse_push_inst(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_MORE_TOKENS;
        return;
    }

    Operand op;

    if (current->token.kind == TT_IMMEDIATE) {
        Word value = current->token.value.as_imm;
        op = IMMEDIATE_OPERAND(value);
    } else if (current->token.kind == TT_REG) {
        Register reg = current->token.value.as_reg;
        op = REGISTER_OPERAND(reg);
    } else {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }
    
    consume_token(parser);

    Inst inst = MAKE_INST_PUSH(op);
    prog_push_inst(&(parser->prog), inst);
}

void parse_pop_inst(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_MORE_TOKENS;
        return;
    }

    Operand op;

    if (current->token.kind == TT_REG) {
        Register reg = current->token.value.as_reg;
        op = REGISTER_OPERAND(reg);
    } else {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }
    
    consume_token(parser);

    Inst inst = MAKE_INST_POP(op);
    prog_push_inst(&(parser->prog), inst);
}

Operand parse_operand(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_MORE_TOKENS;
        return (Operand) {0};
    }

    Operand op;

    if (current->token.kind == TT_REG) {
        Register reg = current->token.value.as_reg;
        op = REGISTER_OPERAND(reg);
    } else if (current->token.kind == TT_IMMEDIATE) {
        Word value = current->token.value.as_imm;
        op = IMMEDIATE_OPERAND(value);
    } else if (current->token.kind == TT_LABEL) {
        char *name = current->token.value.as_str;
        op = LABEL_OPERAND(name);
    } else {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return (Operand) {0};
    } 

    consume_token(parser);
    return op;
}

void parse_arithmetic_inst(Parser *parser, Inst_Type inst_type) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_SEMI_COLON;
        return;
    }

    Operand op1 = NO_TYPE_OPERAND, op2 = NO_TYPE_OPERAND;

    if (current->token.kind == TT_REG) {
        op1 = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op1.type == IMMEDIATE || op1.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }

        if (!expect_token(parser, TT_COMMA)) {
            parser->err = Parser_ERROR_EXPECTED_COMMA;
            return;
        } else {
            // consuming the comma
            consume_token(parser);
        }

        op2 = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op2.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }
    } else if (current->token.kind == TT_LABEL || TT_IMMEDIATE) {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }

    Inst inst;
    
    switch(inst_type) {
        case INST_ADD:
            inst = MAKE_INST_ADD(op1, op2);
            break;
        case INST_SUB:
            inst = MAKE_INST_SUB(op1, op2);
            break;
        case INST_MUL:
            inst = MAKE_INST_MUL(op1, op2);
            break;
        case INST_DIV:
            inst = MAKE_INST_DIV(op1, op2);
            break;
        default:
            printf("IMPOSSIBLE\n");
            break;
    }

    prog_push_inst(&(parser->prog), inst);
}

void parse_halt_inst(Parser *parser) {
    prog_push_inst(&(parser->prog), MAKE_INST_HALT);
}

void parse_inc_inst(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_SEMI_COLON;
        return;
    }

    Operand op = NO_TYPE_OPERAND;   

    if (current->token.kind == TT_REG) {
        op = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op.type == IMMEDIATE || op.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }
    } else if (current->token.kind == TT_LABEL || TT_IMMEDIATE) {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }

    prog_push_inst(&(parser->prog), MAKE_INST_INC(op));
}


void parse_dec_inst(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_SEMI_COLON;
        return;
    }

    Operand op = NO_TYPE_OPERAND;   

    if (current->token.kind == TT_REG) {
        op = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op.type == IMMEDIATE || op.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }
    } else if (current->token.kind == TT_LABEL || TT_IMMEDIATE) {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }

    prog_push_inst(&(parser->prog), MAKE_INST_DEC(op));
}

void parse_cmp_inst(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_SEMI_COLON;
        return;
    }

    Operand op1 = NO_TYPE_OPERAND, op2 = NO_TYPE_OPERAND;

    if (current->token.kind == TT_REG) {
        op1 = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op1.type == IMMEDIATE || op1.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }

        if (!expect_token(parser, TT_COMMA)) {
            parser->err = Parser_ERROR_EXPECTED_COMMA;
            return;
        } else {
            // consuming the comma
            consume_token(parser);
        }

        op2 = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op2.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }
    } else if (current->token.kind == TT_LABEL || TT_IMMEDIATE) {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }

    prog_push_inst(&(parser->prog), MAKE_INST_CMP(op1, op2));
}


void parse_jmp_inst(Parser *parser, Inst_Type type) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_SEMI_COLON;
        return;
    }

    Operand op = NO_TYPE_OPERAND;   

    if (current->token.kind == TT_LABEL) {
        op = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op.type != LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }
    }
    
    Inst inst;

    switch(type) {
        case INST_JMP:
            inst = MAKE_INST_JMP(op);
            break;
        case INST_JZ:
            inst = MAKE_INST_JZ(op);
            break;
        case INST_JGE:
            inst = MAKE_INST_JGE(op);
            break;
        case INST_JLE:
            inst = MAKE_INST_JLE(op);
            break;
        case INST_JL:
            inst = MAKE_INST_JL(op);
            break;
        case INST_JG:
            inst = MAKE_INST_JG(op);
            break;
        case INST_CALL:
            inst = MAKE_INST_CALL(op);
            break;
        default:    
            fprintf(stderr, "Not Implemented yet\n");
            exit(1);
    }

    prog_push_inst(&(parser->prog), inst);
}

void parse_mov_inst(Parser *parser) {
    Token_Node *current = parser->current;
    
    if (current == NULL) {
        parser->err = Parser_ERROR_EXPECTED_SEMI_COLON;
        return;
    }

    Operand op1 = NO_TYPE_OPERAND, op2 = NO_TYPE_OPERAND;

    if (current->token.kind == TT_REG) {
        op1 = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op1.type == IMMEDIATE || op1.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }

        if (!expect_token(parser, TT_COMMA)) {
            parser->err = Parser_ERROR_EXPECTED_COMMA;
            return;
        } else {
            // consuming the comma
            consume_token(parser);
        }

        op2 = parse_operand(parser);

        if (parser->err != Parser_NO_ERROR) return;

        if (op2.type == LABEL) {
            parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
            return;
        }
    } else {
        parser->err = Parser_ERROR_INVALID_TOKEN_TYPE;
        return;
    }

    prog_push_inst(&(parser->prog), MAKE_INST_MOV(op1, op2));
}

void parse_ret_inst(Parser *parser) {
    prog_push_inst(&(parser->prog), MAKE_INST_RET);
}

void parse_syscall_inst(Parser *parser) {
    prog_push_inst(&(parser->prog), MAKE_INST_SYSCALL);
}


void parse_instruction(Parser *parser) {
    Token_Node *current = parser->current;
    consume_token(parser);

    switch(current->token.value.as_inst) {
        case INST_PUSH:
            parse_push_inst(parser);
            break;
        case INST_HALT:
            parse_halt_inst(parser);
            break;
        case INST_POP:
            parse_pop_inst(parser);
            break;
        case INST_ADD:
        case INST_SUB:
        case INST_MUL:
        case INST_DIV:
            parse_arithmetic_inst(parser, current->token.value.as_inst);
            break;
        case INST_INC:
            parse_inc_inst(parser);
            break;
        case INST_DEC:
            parse_dec_inst(parser);
            break;
        case INST_CMP:
            parse_cmp_inst(parser);
            break;
        case INST_JZ:
        case INST_JMP:
        case INST_JLE:
        case INST_JGE:
        case INST_JG:
        case INST_JL:
        case INST_CALL:
            parse_jmp_inst(parser, current->token.value.as_inst);
            break;
        case INST_MOV:
            parse_mov_inst(parser);
            break;
        case INST_RET:
            parse_ret_inst(parser);
            break;
        case INST_SYSCALL:
            parse_syscall_inst(parser);
            break;
        default:
            fprintf(stderr, "NOT IMPLEMENTED YET\n");
            exit(1);
    }       
}

void add_label(Parser *parser) {
    Token_Node *current = parser->current;
    char *name = current->token.value.as_str;

    parser->prog.labels[parser->prog.label_count++] = (Label) {
        .name = name,
        .address = parser->prog.prog_size,
    };

    consume_token(parser);
}

void parse(Parser *parser) {
    while (parser->current != NULL) {
        Token_Node *current = parser->current;

        switch(current->token.kind) {
            case TT_INST:
                parse_instruction(parser);
                break;
            case TT_LABEL_DEF:
                add_label(parser);
                break;
            default:  
                fprintf(stderr, "NOT IMPLEMENTED YET\n");
                exit(1); 
        }

        if (parser->err != Parser_NO_ERROR) {
            fprintf(stderr, "ERROR: %s\n", errors[parser->err]);
            fprintf(stderr, "row: %zu\n", current->token.loc.row);
            exit(1);
        }
    }        
}


#endif