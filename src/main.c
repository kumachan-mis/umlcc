#include "./lexer/lexer.h"
#include "./parser/parser.h"
#include "./gen/gen.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: umlcc [src] [dest]\n");
        exit(1);
    }

    FILE* src = fopen(argv[1], "r");
    FILE* dest = fopen(argv[2], "w");

    Lexer* lexer = new_lexer(src);

    Parser* parser = new_parser(lexer_read_tokens(lexer));
    delete_lexer(lexer);

    Codegen* codegen = new_codegen(parser_create_ast(parser));
    delete_parser(parser);

    Vector* codes = codegen_generate_code(codegen);

    fprintf(dest, "    .text\n");
    fprintf(dest, "    .global _main\n");
    fprintf(dest, "_main:\n");
    fprintf(dest, "    pushq  %%rbp\n");
    fprintf(dest, "    movq  %%rsp, %%rbp\n");

    int codes_len = vector_size(codes);
    for (int i = 0; i < codes_len; i++) {
        fprintf(dest, "%s", vector_at(codes, i));
    }

    fprintf(dest, "    popq  %%rax\n");
    fprintf(dest, "    popq  %%rbp\n");
    fprintf(dest, "    ret\n");

    delete_vector(codes, free);
    fclose(dest);
    fclose(src);
    return 0;
}
