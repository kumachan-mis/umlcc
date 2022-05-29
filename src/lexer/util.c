#include "./util.h"
#include "../common/type.h"

#include <ctype.h>
#include <stdlib.h>

void ctoken_map_add(Map* ctoken_map, char* ctoken_str, CTokenType type);

Map* new_keyword_map() {
    Map* keyword_map = new_map(&t_hashable_string, &t_integer);

    ctoken_map_add(keyword_map, "int", CTOKEN_KEYWORD_INT);
    ctoken_map_add(keyword_map, "return", CTOKEN_KEYWORD_RETURN);

    return keyword_map;
}

Map* new_punctuator_map() {
    Map* punctuator_map = new_map(&t_hashable_string, &t_integer);

    ctoken_map_add(punctuator_map, "[", CTOKEN_LBRACKET);
    ctoken_map_add(punctuator_map, "]", CTOKEN_RBRACKET);
    ctoken_map_add(punctuator_map, "{", CTOKEN_LBRACE);
    ctoken_map_add(punctuator_map, "}", CTOKEN_RBRACE);
    ctoken_map_add(punctuator_map, "(", CTOKEN_LPALEN);
    ctoken_map_add(punctuator_map, ")", CTOKEN_RPALEN);
    ctoken_map_add(punctuator_map, "&", CTOKEN_AND);
    ctoken_map_add(punctuator_map, "*", CTOKEN_ASTERISK);
    ctoken_map_add(punctuator_map, "+", CTOKEN_PLUS);
    ctoken_map_add(punctuator_map, "-", CTOKEN_MINUS);
    ctoken_map_add(punctuator_map, "!", CTOKEN_EXCLAM);
    ctoken_map_add(punctuator_map, "/", CTOKEN_SLASH);
    ctoken_map_add(punctuator_map, "%", CTOKEN_PERCENT);
    ctoken_map_add(punctuator_map, "==", CTOKEN_EQUAL_EQUAL);
    ctoken_map_add(punctuator_map, "!=", CTOKEN_EXCLAM_EQUAL);
    ctoken_map_add(punctuator_map, "&&", CTOKEN_AND_AND);
    ctoken_map_add(punctuator_map, "||", CTOKEN_VBAR_VBAR);
    ctoken_map_add(punctuator_map, ";", CTOKEN_SEMICOLON);
    ctoken_map_add(punctuator_map, "=", CTOKEN_EQUAL);
    ctoken_map_add(punctuator_map, ",", CTOKEN_COMMA);

    return punctuator_map;
}

void ctoken_map_add(Map* ctoken_map, char* ctoken_str, CTokenType type) {
    char* key = new_string(ctoken_str);
    CTokenType* value = malloc(sizeof(CTokenType));
    *value = type;
    map_add(ctoken_map, key, value);
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    while (isspace(c)) {
        c = fgetc(lexer->file_ptr);
    }
    ungetc(c, lexer->file_ptr);
}
