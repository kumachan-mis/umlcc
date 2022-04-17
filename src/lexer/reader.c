#include "./reader.h"
#include "./util.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int keyword_or_identifier_startswith(char c);
int keyword_or_identifier_consistsof(char c);
int integer_constant_startswith(char c);
int integer_constant_consistsof(char c);

Token* read_keyword_or_identifier(Lexer* lexer) {
    char* keyword_or_identifier_str = read_token_str(
        lexer, keyword_or_identifier_startswith, keyword_or_identifier_consistsof
    );
    if (keyword_or_identifier_str == NULL) return NULL;

    if (strcmp(keyword_or_identifier_str, "int") == 0) {
        free(keyword_or_identifier_str);
        return new_token(TOKEN_KEYWORD_INT);
    }

    return new_identifier_token(keyword_or_identifier_str);
}

int keyword_or_identifier_startswith(char c) {
    return isalpha(c) || c == '_';
}

int keyword_or_identifier_consistsof(char c) {
    return isdigit(c) || isalpha(c) || c == '_';
}

Token* read_integer_constant(Lexer* lexer) {
    char* integer_str = read_token_str(
        lexer, integer_constant_startswith, integer_constant_consistsof
    );
    if (integer_str == NULL) return NULL;

    Token* token = new_integer_token(atoi(integer_str));
    free(integer_str);
    return token;
}

int integer_constant_startswith(char c) {
    return isdigit(c);
}

int integer_constant_consistsof(char c) {
    return isdigit(c);
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
        case '=':
            token = new_token(TOKEN_EQUAL);
            break;
        case ',':
            token = new_token(TOKEN_COMMA);
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
