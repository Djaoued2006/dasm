#ifndef LEXER_H
#define LEXER_H

#include "../dm/defs.h"
#include "./defs.h"
#include "./sv.h"
#include "./utils.h"
#include "./global.h"

#define ARR_SIZE(xs) (int)(sizeof(xs)/sizeof((xs)[0]))

typedef enum {
    TT_NONE = 0,
    TT_INST,
    TT_LABEL,
    TT_REG,
    TT_NEW_LINE,
    TT_COMMA,
    TT_IMMEDIATE,
    TT_LABEL_DEF,
} Token_Kind;   

typedef union {
    Inst_Type as_inst;
    char *as_str;
    Word as_imm;
    Register as_reg;
} Token_Value;

typedef struct {
    size_t row;
    size_t col;
} Location;

typedef struct {
    Token_Kind kind;
    Token_Value value;
    Location loc;
} Token;

typedef struct Token_Node{
    Token token;
    struct Token_Node *next;
} Token_Node;

typedef struct {
    Token_Node *head, *tail;
} Tokens;

typedef struct {
    View input;
    size_t current;
    size_t row;
    size_t col;
} Lexer;

Lexer lexer_init(View input) {
    return (Lexer) {
        .input = input,
        .current = 0,
        .row = 1,
        .col = 1,
    };
}

int lexer_end(Lexer *lexer) {
    return (lexer->current == lexer->input.size);
}

char lexer_peek(Lexer *lexer) {
    return (lexer->input.content[lexer->current]);
}

void lexer_consume(Lexer *lexer) {
    lexer->current++;
    lexer->col++;
}

int get_reg_from_cstr(char *content) {
    for (int i = 0; i < ARR_SIZE(registers); ++i) {
        if (strcmp(content, registers[i]) == 0) return i;
    }
    return -1;
}

int get_inst_from_cstr(char *content) {
    for (int i = 0; i < ARR_SIZE(instructions); ++i) {
        if (strcmp(content, instructions[i]) == 0) return i;
    }
    return -1;
}

Token_Kind get_token_kind_from_cstr(char *content) {
    if (get_reg_from_cstr(content) != -1) return TT_REG;
    if (get_inst_from_cstr(content) != -1) return TT_INST;
    return TT_LABEL;
}

Token lexer_next(Lexer *lexer) {
    char *begin = lexer->current + lexer->input.content;
    size_t row = lexer->row;
    size_t col = lexer->col;

    Token_Kind kind;
    Token token;
    token.loc = (Location) { .row = row, .col = col };
    
    char ch = lexer_peek(lexer);

    if (iscomma(ch)) {
        token.kind = TT_COMMA;
        lexer_consume(lexer);
        return token;
    }

    size_t size = 1;    
    
    if (isnumber(ch)) {
        token.kind = TT_IMMEDIATE;
        size = 0;

        while (!lexer_end(lexer)) {
            if (!isnumber(lexer_peek(lexer))) break;
            size++;
            lexer_consume(lexer);
        }   
        
        View number = view_init(begin);
        number.size = size;
        
        char *value = view_to_cstr(number);
        token.value.as_imm = atoll(value);
        free(value);
        return token;
    }
    
    if (ischar(ch)) {
        size = 0;
        while (!lexer_end(lexer)) {
            ch = lexer_peek(lexer);
            if (ch == ':') { size++; lexer_consume(lexer); break; };
            if (!(ischar(ch) || isnumber(ch))) break;
            lexer_consume(lexer);
            size++;
        }
        View current = view_init(begin);
        current.size = size;
        char *content = view_to_cstr(current);

        Token_Kind kind;

        // getting the toke type
        if (content[current.size - 1] == ':') {
            kind = TT_LABEL_DEF;
            
            // put a null terminator to eleminate the ':'
            content[current.size -  1] = '\0';
        } else kind = get_token_kind_from_cstr(content);

        if (kind == TT_NONE) {
            fprintf(stderr, "token kind not implemented yet\n");
            free(content);
            exit(1);
        }

        token.kind = kind;

        if (kind == TT_INST) {
            token.value.as_inst = (Inst_Type) get_inst_from_cstr(content);
        } else if (kind == TT_REG) {
            token.value.as_reg = (Register) get_reg_from_cstr(content);
        } else if (kind == TT_LABEL || kind == TT_LABEL_DEF) {
            token.value.as_str = content;
        }

        return token;
    }


    fprintf(stderr, "ERROR: Invalid Token Found %c, at row: %zu, col: %zu\n", ch, row, col);
    exit(1);
}

void tokens_add(Tokens *tokens, Token token) {
    Token_Node *new_node = (Token_Node *)malloc(sizeof(Token_Node));
    
    new_node->token = token;
    new_node->next = NULL;

    if (tokens->head == NULL) { tokens->head = new_node; }
    else tokens->tail->next = new_node;

    tokens->tail = new_node;
}


Tokens lex(Lexer *lexer) {
    Token token;
    Tokens list = {0};

    while (!lexer_end(lexer)) {
        char ch =  lexer_peek(lexer);
        if (isnewline(ch) || istab(ch) || iswhitespace(ch)) {
            lexer_consume(lexer);
            if (isnewline(ch)) {
                lexer->row++;
                lexer->col = 1;
            }
        } else {
            token = lexer_next(lexer);
            tokens_add(&list, token);
        }
    }

    return list;
}


#endif