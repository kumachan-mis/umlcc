#include "./util.h"
#include "../common/type.h"

#include <stdlib.h>

void ctoken_map_add(Map* ctoken_map, char* ctoken_str, CTokenType type);

Map* new_keyword_map() {
    Map* keyword_map = new_map(&t_hashable_string, &t_integer);

    ctoken_map_add(keyword_map, "char", CTOKEN_KEYWORD_CHAR);
    ctoken_map_add(keyword_map, "int", CTOKEN_KEYWORD_INT);
    ctoken_map_add(keyword_map, "return", CTOKEN_KEYWORD_RETURN);

    return keyword_map;
}

Set* new_nondigit_set() {
    Set* nondigit_set = new_set(&t_hashable_integer);

    int NUM_NONDIGITS = 54;
    char nondigits[54] = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < NUM_NONDIGITS; i++) {
        set_add(nondigit_set, new_integer(nondigits[i]));
    }

    return nondigit_set;
}

Map* new_octdigit_map() {
    Map* octdigit_map = new_map(&t_hashable_integer, &t_integer);

    int NUM_OCTDIGITS = 8;
    char octdigits[9] = "01234567";

    for (int i = 0; i < NUM_OCTDIGITS; i++) {
        map_add(octdigit_map, new_integer(octdigits[i]), new_integer(i));
    }

    return octdigit_map;
}

Map* new_digit_map() {
    Map* digit_map = new_map(&t_hashable_integer, &t_integer);

    int NUM_DIGITS = 10;
    char digits[11] = "0123456789";

    for (int i = 0; i < NUM_DIGITS; i++) {
        map_add(digit_map, new_integer(digits[i]), new_integer(i));
    }

    return digit_map;
}

Map* new_hexdigit_map() {
    Map* hexdigit_map = new_map(&t_hashable_integer, &t_integer);

    int NUM_DIGITS = 10;
    char digits[11] = "0123456789";

    for (int i = 0; i < NUM_DIGITS; i++) {
        map_add(hexdigit_map, new_integer(digits[i]), new_integer(i));
    }

    int NUM_HEXDIGITS = 6;
    char lower_hexdigits[7] = "abcdef";
    char upper_hexdigits[7] = "ABCDEF";

    for (int i = 0; i < NUM_HEXDIGITS; i++) {
        map_add(hexdigit_map, new_integer(lower_hexdigits[i]), new_integer(i + 10));
        map_add(hexdigit_map, new_integer(upper_hexdigits[i]), new_integer(i + 10));
    }

    return hexdigit_map;
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

Set* new_white_space_set() {
    Set* white_space_set = new_set(&t_hashable_integer);

    int NUM_WHITE_SPACES = 6;
    char spaces[6] = " \t\n\v\f";

    for (int i = 0; i < NUM_WHITE_SPACES; i++) {
        set_add(white_space_set, new_integer(spaces[i]));
    }

    return white_space_set;
}

void ctoken_map_add(Map* ctoken_map, char* ctoken_str, CTokenType type) {
    char* key = new_string(ctoken_str);
    CTokenType* value = malloc(sizeof(CTokenType));
    *value = type;
    map_add(ctoken_map, key, value);
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    while (set_contains(lexer->white_space_set, &c)) {
        c = fgetc(lexer->file_ptr);
    }
    ungetc(c, lexer->file_ptr);
}
