#include "./lexer.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Token* read_integer_constant(Lexer* lexer);
Token* read_punctuator(Lexer* lexer);
void   skip_white_spaces(Lexer* lexer);

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

Token* read_integer_constant(Lexer* lexer) {
    int capacity = 1, length = 0;
    char* integer_str = malloc(sizeof(char) * capacity);

    while (1) {
        int c = fgetc(lexer->_file_ptr);
        if (!isdigit(c)) {
            integer_str[length] = '\0';
            ungetc(c, lexer->_file_ptr);
            break;
        }

        integer_str[length] = c;
        length++;
        if (length >= capacity) {
            integer_str = realloc(integer_str, 2 * capacity * sizeof(char));
            capacity *= 2;
        }
    }

    Token* token = NULL;
    if (length > 0) token = new_integer_token(atoi(integer_str));

    free(integer_str);
    return token;
}

Token* read_punctuator(Lexer* lexer) {
    Token* token = NULL;

    int c = fgetc(lexer->_file_ptr);
    switch (c) {
        case '{':
            token = new_token(TOKEN_LBRACE);
            break;
        case '}':
            token = new_token(TOKEN_RBRACE);
            break;
        case '(':
            token = new_token(TOKEN_LPALEN);
            break;
        case ')':
            token = new_token(TOKEN_RPALEN);
            break;
        case '+': 
            token = new_token(TOKEN_PLUS);
            break;
        case '-': 
            token = new_token(TOKEN_MINUS);
            break;
        case '*': 
            token = new_token(TOKEN_ASTERISK);
            break;
        case '/':
            token = new_token(TOKEN_SLASH);
            break;
        case '%':
            token = new_token(TOKEN_PERCENT);
            break;
         case ';':
            token = new_token(TOKEN_SEMICOLON);
            break;
        case EOF: 
            token = new_token(TOKEN_EOF);
            break;
        default: 
            ungetc(c, lexer->_file_ptr);
            token = NULL;
            break;
    }

    return token;
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->_file_ptr);
    while (isspace(c)) {
        c = fgetc(lexer->_file_ptr);
    }
    ungetc(c, lexer->_file_ptr);
}
