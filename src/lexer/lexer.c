#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include <stdlib.h>
#include <string.h>

Lexer* new_lexer(char* content) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->content = content;
    lexer->content_index = 0L;
    lexer->content_size = strlen(content);
    lexer->keyword_map = new_keyword_map();
    lexer->nondigit_set = new_nondigit_set();
    lexer->octdigit_map = new_octdigit_map();
    lexer->digit_map = new_digit_map();
    lexer->hexdigit_map = new_hexdigit_map();
    lexer->integer_suffix_map = new_integer_suffix_map();
    lexer->punctuator_map = new_punctuator_map();
    lexer->white_space_set = new_white_space_set();
    return lexer;
}

LexerReturn* lexer_read_ctokens(Lexer* lexer) {
    Vector* ctokens = new_vector(&t_ctoken);
    CToken* ctoken = NULL;
    Error* err = NULL;

    skip_white_spaces(lexer);

    while (1) {
        LexerReturnItem* item = NULL;

        int c = content_top(lexer);

        if (set_contains(lexer->nondigit_set, &c)) {
            item = read_keyword_or_identifier(lexer);
        } else if (map_contains(lexer->digit_map, &c)) {
            item = read_integer_constant(lexer);
        } else if (c == '\'') {
            item = read_character_constant(lexer);
        } else if (c == '\"') {
            item = read_string_literal(lexer);
        } else {
            item = read_punctuator(lexer);
        }

        lexerret_item_assign(&ctoken, &err, item);

        if (err != NULL) {
            delete_vector(ctokens);
            return new_lexerret_error(err);
        }
        vector_push(ctokens, ctoken);

        if (ctoken->type == CTOKEN_EOF) {
            break;
        }
        skip_white_spaces(lexer);
    }

    return new_lexerret(ctokens);
}

void delete_lexer(Lexer* lexer) {
    free(lexer->content);
    delete_map(lexer->keyword_map);
    delete_set(lexer->nondigit_set);
    delete_map(lexer->octdigit_map);
    delete_map(lexer->digit_map);
    delete_map(lexer->hexdigit_map);
    delete_map(lexer->integer_suffix_map);
    delete_map(lexer->punctuator_map);
    delete_set(lexer->white_space_set);
    free(lexer);
}
