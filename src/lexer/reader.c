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
    TokenType* token_ref = map_get(lexer->_keyword_map, token_str);

    if (token_ref != NULL) {
        free(token_str);
        return new_token(*token_ref);
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
    int MAX_LENGTH = 3;

    char* token_str = malloc((MAX_LENGTH + 1) * sizeof(int));
    int length = 0;
    memset(token_str, 0, MAX_LENGTH + 1);

    int c = fgetc(lexer->_file_ptr);
    if (c == EOF) {
        free(token_str);
        return new_token(TOKEN_EOF);
    }
    token_str[length] = c;
    length++;

    for (int i = 0; i < MAX_LENGTH - 1; i++) {
        c = fgetc(lexer->_file_ptr);
        if (c == EOF) {
            ungetc(c, lexer->_file_ptr);
            break;
        }
        token_str[length] = c;
        length++;
    }

    while (length > 0) {
        TokenType* token_ref = map_get(lexer->_punctuator_map, token_str);
        if (token_ref != NULL) {
            free(token_str);
            return new_token(*token_ref);
        }
        length--;
        ungetc(token_str[length], lexer->_file_ptr);
        token_str[length] = '\0';
    }

    free(token_str);
    return NULL;
}
