#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include <stdlib.h>

Map* create_keyword_map();

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->_file_ptr = file_ptr;
    lexer->_keyword_map = create_keyword_map();
    return lexer;
}

void delete_lexer(Lexer* lexer) {
    delete_map(lexer->_keyword_map, free);
    free(lexer);
}

Vector* lexer_read_tokens(Lexer* lexer) {
    Vector* tokens = new_vector();
    skip_white_spaces(lexer);

    while (1) {
        Token* token = NULL;
        if (token == NULL) token = read_keyword_or_identifier(lexer);
        if (token == NULL) token = read_integer_constant(lexer);
        if (token == NULL) token = read_punctuator(lexer);

        if (token == NULL) {
            int c = fgetc(lexer->_file_ptr);
            fprintf(stderr, "Error: unexpected character %c\n", c);
            exit(1);
        }

        vector_push(tokens, token);

        if (token->type == TOKEN_EOF) break;
        skip_white_spaces(lexer);
    }

    return tokens;
}

Map* create_keyword_map() {
    Map* keyword_map = new_map();
    TokenType* keyword_ref = NULL;

    keyword_ref = malloc(sizeof(TokenType));
    *keyword_ref = TOKEN_KEYWORD_INT;
    map_set(keyword_map, "int", keyword_ref, free);

    keyword_ref = malloc(sizeof(TokenType));
    *keyword_ref = TOKEN_KEYWORD_RETURN;
    map_set(keyword_map, "return", keyword_ref, free);

    return keyword_map;
}
