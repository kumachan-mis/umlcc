#include "./util.h"

#include <ctype.h>
#include <stdlib.h>

void token_map_add(Map* token_map, char* token_str, TokenType type);

Map* new_keyword_map() {
    Map* keyword_map = new_map();

    token_map_add(keyword_map, "int", TOKEN_KEYWORD_INT);
    token_map_add(keyword_map, "return", TOKEN_KEYWORD_RETURN);

    return keyword_map;
}

Map* new_punctuator_map() {
    Map* punctuator_map = new_map();

    token_map_add(punctuator_map, "{", TOKEN_LBRACE);
    token_map_add(punctuator_map, "}", TOKEN_RBRACE);
    token_map_add(punctuator_map, "(", TOKEN_LPALEN);
    token_map_add(punctuator_map, ")", TOKEN_RPALEN);
    token_map_add(punctuator_map, "*", TOKEN_ASTERISK);
    token_map_add(punctuator_map, "+", TOKEN_PLUS);
    token_map_add(punctuator_map, "-", TOKEN_MINUS);
    token_map_add(punctuator_map, "!", TOKEN_EXCLAM);
    token_map_add(punctuator_map, "/", TOKEN_SLASH);
    token_map_add(punctuator_map, "%", TOKEN_PERCENT);
    token_map_add(punctuator_map, "&&", TOKEN_AND_AND);
    token_map_add(punctuator_map, "||", TOKEN_VBAR_VBAR);
    token_map_add(punctuator_map, ";", TOKEN_SEMICOLON);
    token_map_add(punctuator_map, "=", TOKEN_EQUAL);
    token_map_add(punctuator_map, ",", TOKEN_COMMA);

    return punctuator_map;
}

void delete_token_map(Map* token_map) {
    delete_map(token_map, free);
}

void token_map_add(Map* token_map, char* token_str, TokenType type) {
    TokenType* token_ref = malloc(sizeof(TokenType));
    *token_ref = type;
    map_set(token_map, token_str, token_ref, free);
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->_file_ptr);
    while (isspace(c)) {
        c = fgetc(lexer->_file_ptr);
    }
    ungetc(c, lexer->_file_ptr);
}
