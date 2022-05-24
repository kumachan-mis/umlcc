#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include <stdlib.h>

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->file_ptr = file_ptr;
    lexer->keyword_map = new_keyword_map();
    lexer->punctuator_map = new_punctuator_map();
    return lexer;
}

void delete_lexer(Lexer* lexer) {
    delete_map(lexer->keyword_map);
    delete_map(lexer->punctuator_map);
    free(lexer);
}

Vector* lexer_read_ctokens(Lexer* lexer) {
    Vector* ctokens = new_vector(&t_ctoken);
    skip_white_spaces(lexer);

    while (1) {
        CToken* ctoken = NULL;
        if (ctoken == NULL) ctoken = read_keyword_or_identifier(lexer);
        if (ctoken == NULL) ctoken = read_integer_constant(lexer);
        if (ctoken == NULL) ctoken = read_punctuator(lexer);

        if (ctoken == NULL) {
            int c = fgetc(lexer->file_ptr);
            fprintf(stderr, "Error: unexpected character %c\n", c);
            exit(1);
        }

        vector_push(ctokens, ctoken);

        if (ctoken->type == CTOKEN_EOF) break;
        skip_white_spaces(lexer);
    }

    return ctokens;
}
