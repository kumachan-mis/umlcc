#include "./gen/gen.h"
#include "./lexer/lexer.h"
#include "./parser/parser.h"
#include "./resolver/resolver.h"

#include "./immc/immc.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: umlcc [src] [dest]\n");
        exit(1);
    }

    FILE* src = fopen(argv[1], "r");
    FILE* dest = fopen(argv[2], "w");

    Lexer* lexer = new_lexer(src);
    Vector* tokens = lexer_read_tokens(lexer);
    delete_lexer(lexer);

    Parser* parser = new_parser(tokens);
    Ast* ast = parser_create_ast(parser);
    delete_parser(parser);

    Resolver* resolver = new_resolver(ast);
    Srt* srt = resolver_resolve_semantics(resolver);
    delete_resolver(resolver);

    Codegen* codegen = new_codegen(srt);
    Vector* immcs = codegen_generate_code(codegen);
    delete_codegen(codegen);

    int immcs_len = vector_size(immcs);
    for (int i = 0; i < immcs_len; i++) {
        char* immc_str = immc_tostring(vector_at(immcs, i));
        fprintf(dest, "%s", immc_str);
        free(immc_str);
    }
    delete_vector(immcs, (void (*)(void* item))delete_immc);

    fclose(dest);
    fclose(src);

    return 0;
}
