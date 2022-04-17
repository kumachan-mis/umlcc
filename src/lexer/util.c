#include "./util.h"

#include <stdlib.h>
#include <ctype.h>


char* read_token_str(Lexer* lexer, int startswith(char c), int consistsof(char c)) {
    int initial_char = fgetc(lexer->_file_ptr);
    ungetc(initial_char, lexer->_file_ptr);

    if (!startswith(initial_char)) return NULL;

    int capacity = 1, length = 0;
    char* token_str = malloc(sizeof(char) * capacity);

    while (1) {
        int c = fgetc(lexer->_file_ptr);
        if (!consistsof(c)) {
            token_str[length] = '\0';
            ungetc(c, lexer->_file_ptr);
            break;
        }

        token_str[length] = c;
        length++;
        if (length >= capacity) {
            token_str = realloc(token_str, 2 * capacity * sizeof(char));
            capacity *= 2;
        }
    }

    token_str = realloc(token_str, (length + 1) * sizeof(char));
    return token_str;
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->_file_ptr);
    while (isspace(c)) {
        c = fgetc(lexer->_file_ptr);
    }
    ungetc(c, lexer->_file_ptr);
}
