#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include <stdlib.h>

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->_file_ptr = file_ptr;
    lexer->_keyword_map = new_keyword_map();
    lexer->_punctuator_map = new_punctuator_map();
    return lexer;
}

void delete_lexer(Lexer* lexer) {
    delete_map(lexer->_keyword_map);
    delete_map(lexer->_punctuator_map);
    free(lexer);
}

Vector* lexer_read_tokens(Lexer* lexer) {
    Vector* tokens = new_vector(&t_token);
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
