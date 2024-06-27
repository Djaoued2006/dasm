#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "./asm.h"
#include "../dm/dm.h"

void machine_load_prog(Machine *dm, Parser *parser) {
    for (size_t i = 0; i < parser->prog.prog_size; ++i) {
        dm->prog.instructions[dm->prog.prog_size++] = parser->prog.instructions[i]; 
    }
    for (size_t i = 0; i < parser->prog.label_count; ++i) {
        dm->prog.labels[dm->prog.label_count++] = parser->prog.labels[i];
    }
}

int main(void) {
    struct termios attr;

    tcgetattr(STDIN_FILENO, &attr);

    attr.c_lflag &= ~ICANON;

    tcsetattr(STDIN_FILENO, TCSANOW, &attr);

    char *content = get_file_content("./test/syscall.asm");
    Lexer lexer = lexer_init(view_init(content));

    Tokens tokens = lex(&lexer);
    Token_Node *current = tokens.head;

    Parser parser = asm_init(tokens);
    Machine dm;

    dm_init(&dm);

    parse(&parser);

    machine_load_prog(&dm, &parser);

    dm_execute(&dm);


    printf("Stack:\n");
    for (size_t i = 0; i < dm.sp; ++i) {
        printf("\t%ld\n", dm.stack[i]);
    }
    
    printf("Registers:\n");
    for (int i = 0; i  < R_COUNT; ++i) {
        printf("%s = %ld\n", registers[(Register) i], dm.registers[i]);
    }

    while(current) {
        Token_Node *next = current->next;
        free(current);
        current = next;
    }
    

    return 0;
}