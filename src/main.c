#include "./immcgen/immcgen.h"
#include "./lexer/lexer.h"
#include "./parser/parser.h"
#include "./regalloc/regalloc.h"
#include "./resolver/resolver.h"
#include "./x64gen/x64gen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Error: no input file\n");
        exit(1);
    }

    FILE* src = fopen(argv[1], "r");
    if (src == NULL) {
        fprintf(stderr, "Error: %s: no such file or directory\n", argv[1]);
        exit(1);
    }

    char* ext = strrchr(argv[1], '.');
    if (ext == NULL || strcmp(ext, ".c") != 0) {
        fprintf(stderr, "Error: file format is not .c\n");
        exit(1);
    }

    int src_filename_len = strlen(argv[1]);

    char* imm_filename = new_string(argv[1]);
    imm_filename[src_filename_len - 1] = 'i';

    char* dst_filename = new_string(argv[1]);
    dst_filename[src_filename_len - 1] = 's';

    FILE* imm = fopen(imm_filename, "w");
    FILE* dst = fopen(dst_filename, "w");

    free(dst_filename);
    free(imm_filename);

    Lexer* lexer = new_lexer(src);
    Vector* ctokens = lexer_read_ctokens(lexer);
    delete_lexer(lexer);

    Parser* parser = new_parser(ctokens);
    Ast* ast = parser_create_ast(parser);
    delete_parser(parser);

    Resolver* resolver = new_resolver(ast);
    Srt* srt = resolver_resolve_semantics(resolver);
    delete_resolver(resolver);

    Immcgen* immcgen = new_immcgen(srt);
    Vector* immcs = immcgen_generate_immcode(immcgen);
    delete_immcgen(immcgen);

    RegAlloc* regalloc = new_regalloc(immcs, NUM_CALLER_SAVED_REGS);
    AllocImmcs* allocimmcs = regalloc_allocate_regs(regalloc);
    delete_regalloc(regalloc);

    Vector *allocated_immcs = NULL, *liveseqs = NULL;
    allocimmcs_move(allocimmcs, &allocated_immcs, &liveseqs);

    int immcs_len = vector_size(allocated_immcs);
    for (int i = 0; i < immcs_len; i++) {
        char* immc_str = immc_tostring(vector_at(allocated_immcs, i));
        fprintf(imm, "%s", immc_str);
        free(immc_str);
    }

    X64gen* x64gen = new_x64gen(allocated_immcs, liveseqs);
    Vector* x64codes = x64gen_generate_x64code(x64gen);
    delete_x64gen(x64gen);

    int x64codes_len = vector_size(x64codes);
    for (int i = 0; i < x64codes_len; i++) {
        fprintf(dst, "%s", (char*)vector_at(x64codes, i));
    }
    delete_vector(x64codes);

    fclose(dst);
    fclose(imm);
    fclose(src);

    return 0;
}
