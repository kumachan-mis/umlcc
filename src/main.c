#include "./gen/gen.h"
#include "./lexer/lexer.h"
#include "./parser/parser.h"
#include "./resolver/resolver.h"

#include "./imml/imml.h"

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
    Vector* immlcodes = codegen_generate_code(codegen);
    delete_codegen(codegen);

    int immlcodes_len = vector_size(immlcodes);
    for (int i = 0; i < immlcodes_len; i++) {
        char* immlcode_str = immlcode_tostring(vector_at(immlcodes, i));
        fprintf(dest, "%s", immlcode_str);
        free(immlcode_str);
    }
    delete_vector(immlcodes, delete_immlcode);

    fclose(dest);
    fclose(src);

    return 0;
}
