#include "./lexer.h"
#include "./reader.h"
#include "./util.h"

#include  <stdlib.h>


Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->_file_ptr = file_ptr;
    return lexer;
}

void delete_lexer(Lexer* lexer) {
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
        } else if (token->type == TOKEN_EOF) {
            vector_push(tokens, token);
            break;
        }
    
        vector_push(tokens, token);
        skip_white_spaces(lexer);
    }
    return tokens;
}

