#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include <stdlib.h>

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->file_ptr = file_ptr;
    lexer->keyword_map = new_keyword_map();
    lexer->nondigit_set = new_nondigit_set();
    lexer->octdigit_map = new_octdigit_map();
    lexer->digit_map = new_digit_map();
    lexer->hexdigit_map = new_hexdigit_map();
    lexer->punctuator_map = new_punctuator_map();
    lexer->white_space_set = new_white_space_set();
    return lexer;
}

Vector* lexer_read_ctokens(Lexer* lexer) {
    Vector* ctokens = new_vector(&t_ctoken);
    skip_white_spaces(lexer);

    while (1) {
        CToken* ctoken = NULL;

        int c = fgetc(lexer->file_ptr);
        ungetc(c, lexer->file_ptr);

        if (set_contains(lexer->nondigit_set, &c)) {
            ctoken = read_keyword_or_identifier(lexer);
        } else if (map_contains(lexer->digit_map, &c)) {
            ctoken = read_integer_constant(lexer);
        } else if (c == '\'') {
            ctoken = read_character_constant(lexer);
        } else if (c == '\"') {
            ctoken = read_string_literal(lexer);
        } else {
            ctoken = read_punctuator(lexer);
        }

        vector_push(ctokens, ctoken);

        if (ctoken->type == CTOKEN_EOF) break;
        skip_white_spaces(lexer);
    }

    return ctokens;
}

void delete_lexer(Lexer* lexer) {
    delete_map(lexer->keyword_map);
    delete_set(lexer->nondigit_set);
    delete_map(lexer->octdigit_map);
    delete_map(lexer->digit_map);
    delete_map(lexer->hexdigit_map);
    delete_map(lexer->punctuator_map);
    delete_set(lexer->white_space_set);
    free(lexer);
}
