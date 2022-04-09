#include "./lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->_file_ptr = file_ptr;
    return lexer;
}

void delete_lexer(Lexer* lexer) {
    free(lexer);
}

Token* read_next_token(Lexer* lexer) {
    skip_white_spaces(lexer);

    int c = fgetc(lexer->_file_ptr);
    if (c == EOF) {
        Token* next_token = new_reverved_token(TOKEN_EOF);
        skip_white_spaces(lexer);
        return next_token;
    }

    if (isdigit(c)) {
        ungetc(c, lexer->_file_ptr);
        Token* next_token = read_next_integer_token(lexer);
        skip_white_spaces(lexer);
        return next_token;
    }

    Token* next_token = NULL;
    switch (c) {
        case '+': {
            next_token = new_reverved_token(TOKEN_PLUS);
        }
        case '-': {
            next_token = new_reverved_token(TOKEN_MINUS);
        }
        case '*': {
            next_token = new_reverved_token(TOKEN_ASTERISK);
        }
        case '/': {
            next_token = new_reverved_token(TOKEN_SLASH);
        }
        case '%': {
            next_token = new_reverved_token(TOKEN_PERCENT);
        }
        default: {
            fprintf(stderr, "Error: unknown character %c", c);
            exit(1);            
        }
    }
    skip_white_spaces(lexer);
    return next_token;
}

Token* read_next_integer_token(Lexer* lexer) {
    int capacity = 1, length = 0;
    char* integer_str = malloc(sizeof(char) * capacity);

    int c = fgetc(lexer->_file_ptr);
    while (isdigit(c)) {
        integer_str[length] = c;
        length++;

        if (length == capacity) {
            char* new_integer_str = malloc(sizeof(char) * (2 * capacity));
            memcpy(new_integer_str, integer_str, length);
            free(integer_str);
            integer_str = new_integer_str;
            capacity *= 2;
        }

        c = fgetc(lexer->_file_ptr);
    }

    integer_str[length] = '\0';
    ungetc(c, lexer->_file_ptr);
    return new_integer_token(atoi(integer_str));
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->_file_ptr);
    while (isspace(c)) {
        c = fgetc(lexer->_file_ptr);
    }
    ungetc(c, lexer->_file_ptr);
}
