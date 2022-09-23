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
        fprintf(stderr, "\x1b[1;31merror\x1b[0m: no input file\n");
        return 1;
    }

    int src_filename_len = strlen(argv[1]);
    FILE* src = fopen(argv[1], "r");

    if (src == NULL) {
        fprintf(stderr, "\x1b[1;31merror\x1b[0m: %s: no such file or directory\n", argv[1]);
        return 1;
    }

    char* ext = strrchr(argv[1], '.');
    if (ext == NULL || strcmp(ext, ".c") != 0) {
        fprintf(stderr, "\x1b[1;31merror\x1b[0m: file format is not .c\n");
        return 1;
    }

    Vector* errs = NULL;
    Error* err = NULL;

    Lexer* lexer = new_lexer(src);
    Vector* ctokens = NULL;
    lexerret_assign(&ctokens, &err, lexer_read_ctokens(lexer));
    delete_lexer(lexer);
    fclose(src);

    if (err != NULL) {
        fprintf(stderr, "\x1b[0;36m@@@@@ Error occured in lexer @@@@@\x1b[0m\n");
        fprintf(stderr, "    \x1b[1;31merror\x1b[0m: %s", err->message);
        delete_error(err);
        return 1;
    }

    Parser* parser = new_parser(ctokens);
    Ast* ast = NULL;
    parserret_assign(&ast, &err, parser_create_ast(parser));
    delete_parser(parser);

    if (err != NULL) {
        fprintf(stderr, "\x1b[0;36m@@@@@ Error occured in parser @@@@@\x1b[0m\n");
        fprintf(stderr, "    \x1b[1;31merror\x1b[0m: %s", err->message);
        delete_error(err);
        return 1;
    }

    Resolver* resolver = new_resolver(ast);
    Srt* srt = NULL;
    resolverret_assign(&srt, &errs, resolver_resolve_semantics(resolver));
    delete_resolver(resolver);

    if (errs != NULL) {
        fprintf(stderr, "\x1b[0;36m@@@@@ Error occured in resolver @@@@@\x1b[0m\n");
        int num_errs = vector_size(errs);
        for (int i = 0; i < num_errs; i++) {
            err = vector_at(errs, i);
            fprintf(stderr, "    \x1b[1;31merror\x1b[0m: %s", err->message);
        }
        delete_vector(errs);
        return 1;
    }

    Immcgen* immcgen = new_immcgen(srt);
    Vector* immcs = immcgen_generate_immcode(immcgen);
    delete_immcgen(immcgen);

    RegAlloc* regalloc = new_regalloc(immcs, NUM_CALLER_SAVED_REGS);
    Vector* allocated_immcs = NULL;
    Vector* liveseqs = NULL;
    regallocret_assign(&allocated_immcs, &liveseqs, regalloc_allocate_regs(regalloc));
    delete_regalloc(regalloc);

    char* imm_filename = new_string(argv[1]);
    imm_filename[src_filename_len - 1] = 'i';
    FILE* imm = fopen(imm_filename, "w");
    free(imm_filename);

    int immcs_len = vector_size(allocated_immcs);
    for (int i = 0; i < immcs_len; i++) {
        char* immc_str = immc_tostring(vector_at(allocated_immcs, i));
        fprintf(imm, "%s", immc_str);
        free(immc_str);
    }
    fclose(imm);

    X64gen* x64gen = new_x64gen(allocated_immcs, liveseqs);
    Vector* x64codes = x64gen_generate_x64code(x64gen);
    delete_x64gen(x64gen);

    char* dst_filename = new_string(argv[1]);
    dst_filename[src_filename_len - 1] = 's';
    FILE* dst = fopen(dst_filename, "w");
    free(dst_filename);

    int x64codes_len = vector_size(x64codes);
    for (int i = 0; i < x64codes_len; i++) {
        char* x64_str = x64_tostring(vector_at(x64codes, i));
        fprintf(dst, "%s", x64_str);
        free(x64_str);
    }
    fclose(dst);

    delete_vector(x64codes);

    return 0;
}
