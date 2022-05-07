#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include <stdlib.h>

void token_map_add(Map* token_map, char* token_str, TokenType type);

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->_file_ptr = file_ptr;
    lexer->_keyword_map = new_keyword_map();
    lexer->_punctuator_map = new_punctuator_map();
    return lexer;
}

void delete_lexer(Lexer* lexer) {
    delete_token_map(lexer->_keyword_map);
    delete_token_map(lexer->_punctuator_map);
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
