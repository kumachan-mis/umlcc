#include "./lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


struct _Lexer {
    FILE* file_ptr;
};

Token* read_integer_constant(Lexer* lexer);
Token* read_punctuator(Lexer* lexer);
void   skip_white_spaces(Lexer* lexer);

Lexer* new_lexer(FILE* file_ptr) {
    Lexer* lexer = malloc(sizeof(Lexer));
    lexer->file_ptr = file_ptr;
    return lexer;
}

void delete_lexer(Lexer* lexer) {
    free(lexer);
}

Token* lexer_read_next_token(Lexer* lexer) {
    Token* next_token = NULL;
    skip_white_spaces(lexer);

    if (next_token != NULL) next_token = read_integer_constant(lexer);
    if (next_token != NULL) next_token = read_punctuator(lexer);
       
    skip_white_spaces(lexer);
    return next_token;
}

Token* read_integer_constant(Lexer* lexer) {
    int capacity = 1, length = 0;
    char* integer_str = malloc(sizeof(char) * capacity);

    int c = fgetc(lexer->file_ptr);
    while (isdigit(c)) {
        integer_str[length] = c;
        length++;

        if (length == capacity) {
            integer_str = realloc(integer_str, 2 * capacity * sizeof(char));
            capacity *= 2;
        }

        c = fgetc(lexer->file_ptr);
    }

    integer_str[length] = '\0';
    ungetc(c, lexer->file_ptr);

    if (length == 0) return NULL;
    return new_integer_token(atoi(integer_str));
}

Token* read_punctuator(Lexer* lexer) {
    Token* next_token = NULL;

    int c = fgetc(lexer->file_ptr);
    switch (c) {
        case '+': {
            next_token = new_token(TOKEN_PLUS);
        }
        case '-': {
            next_token = new_token(TOKEN_MINUS);
        }
        case '*': {
            next_token = new_token(TOKEN_ASTERISK);
        }
        case '/': {
            next_token = new_token(TOKEN_SLASH);
        }
        case '%': {
            next_token = new_token(TOKEN_PERCENT);
        }
        case EOF: {
            next_token = new_token(TOKEN_EOF);
        }
    }

    return next_token;
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    while (isspace(c)) {
        c = fgetc(lexer->file_ptr);
    }
    ungetc(c, lexer->file_ptr);
}
