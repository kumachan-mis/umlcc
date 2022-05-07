#include "./reader.h"
#include "./dystring.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

Token* read_keyword_or_identifier(Lexer* lexer) {
    DyString* dystring = new_dystring();

    char c = fgetc(lexer->_file_ptr);
    if (!isalpha(c) && c != '_') {
        ungetc(c, lexer->_file_ptr);
        return NULL;
    }

    dystring_push(dystring, c);
    while (1) {
        c = fgetc(lexer->_file_ptr);
        if (!isalpha(c) && !isdigit(c) && c != '_') {
            ungetc(c, lexer->_file_ptr);
            break;
        }
        dystring_push(dystring, c);
    }

    char* token_str = dystring_finish(dystring);
    TokenType* keyword_ref = map_get(lexer->_keyword_map, token_str);

    if (keyword_ref != NULL) {
        free(token_str);
        return new_token(*keyword_ref);
    }
    return new_identifier_token(token_str);
}

Token* read_integer_constant(Lexer* lexer) {
     DyString* dystring = new_dystring();

    char c = fgetc(lexer->_file_ptr);
    if (!isdigit(c)) {
        ungetc(c, lexer->_file_ptr);
        return NULL;
    }

    dystring_push(dystring, c);
    while (1) {
        c = fgetc(lexer->_file_ptr);
        if (!isdigit(c)) {
            ungetc(c, lexer->_file_ptr);
            break;
        }
        dystring_push(dystring, c);
    }
    
    char* token_str = dystring_finish(dystring);
    Token* token = new_integer_token(atoi(token_str));
    free(token_str);

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
