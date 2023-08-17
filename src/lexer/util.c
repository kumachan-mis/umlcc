#include "./util.h"
#include "../common/type.h"

#include <stdlib.h>

void integer_suffix_map_add(Map* integer_suffix_map, char* suffix_str, IntegerLiteralType type);
void ctoken_map_add(Map* ctoken_map, char* ctoken_str, CTokenType type);

Map* new_keyword_map(void) {
    Map* keyword_map = new_map(&t_hashable_string, &t_integer);

    ctoken_map_add(keyword_map, "char", CTOKEN_KEYWORD_CHAR);
    ctoken_map_add(keyword_map, "enum", CTOKEN_KEYWORD_ENUM);
    ctoken_map_add(keyword_map, "int", CTOKEN_KEYWORD_INT);
    ctoken_map_add(keyword_map, "long", CTOKEN_KEYWORD_LONG);
    ctoken_map_add(keyword_map, "return", CTOKEN_KEYWORD_RETURN);
    ctoken_map_add(keyword_map, "sizeof", CTOKEN_KEYWORD_SIZEOF);
    ctoken_map_add(keyword_map, "struct", CTOKEN_KEYWORD_STRUCT);
    ctoken_map_add(keyword_map, "typedef", CTOKEN_KEYWORD_TYPEDEF);
    ctoken_map_add(keyword_map, "unsigned", CTOKEN_KEYWORD_UNSIGNED);
    ctoken_map_add(keyword_map, "void", CTOKEN_KEYWORD_VOID);

    return keyword_map;
}

Set* new_nondigit_set(void) {
    Set* nondigit_set = new_set(&t_hashable_integer);

    int NUM_NONDIGITS = 54;
    char nondigits[54] = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < NUM_NONDIGITS; i++) {
        set_add(nondigit_set, new_integer(nondigits[i]));
    }

    return nondigit_set;
}

Map* new_octdigit_map(void) {
    Map* octdigit_map = new_map(&t_hashable_integer, &t_integer);

    int NUM_OCTDIGITS = 8;
    char octdigits[9] = "01234567";

    for (int i = 0; i < NUM_OCTDIGITS; i++) {
        map_add(octdigit_map, new_integer(octdigits[i]), new_integer(i));
    }

    return octdigit_map;
}

Map* new_digit_map(void) {
    Map* digit_map = new_map(&t_hashable_integer, &t_integer);

    int NUM_DIGITS = 10;
    char digits[11] = "0123456789";

    for (int i = 0; i < NUM_DIGITS; i++) {
        map_add(digit_map, new_integer(digits[i]), new_integer(i));
    }

    return digit_map;
}

Map* new_hexdigit_map(void) {
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

Map* new_integer_suffix_map(void) {
    Map* integer_suffix_map = new_map(&t_hashable_string, &t_integer);

    integer_suffix_map_add(integer_suffix_map, "l", INTEGER_LONG);
    integer_suffix_map_add(integer_suffix_map, "L", INTEGER_LONG);

    integer_suffix_map_add(integer_suffix_map, "ll", INTEGER_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "LL", INTEGER_LONGLONG);

    integer_suffix_map_add(integer_suffix_map, "u", INTEGER_UNSIGNED_INT);
    integer_suffix_map_add(integer_suffix_map, "U", INTEGER_UNSIGNED_INT);

    integer_suffix_map_add(integer_suffix_map, "ul", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "Ul", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "uL", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "UL", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "lu", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "Lu", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "lU", INTEGER_UNSIGNED_LONG);
    integer_suffix_map_add(integer_suffix_map, "LU", INTEGER_UNSIGNED_LONG);

    integer_suffix_map_add(integer_suffix_map, "ull", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "Ull", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "uLL", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "ULL", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "llu", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "LLu", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "llU", INTEGER_UNSIGNED_LONGLONG);
    integer_suffix_map_add(integer_suffix_map, "LLU", INTEGER_UNSIGNED_LONGLONG);

    return integer_suffix_map;
}

Map* new_punctuator_map(void) {
    Map* punctuator_map = new_map(&t_hashable_string, &t_integer);

    ctoken_map_add(punctuator_map, "[", CTOKEN_LBRACKET);
    ctoken_map_add(punctuator_map, "]", CTOKEN_RBRACKET);
    ctoken_map_add(punctuator_map, "(", CTOKEN_LPALEN);
    ctoken_map_add(punctuator_map, ")", CTOKEN_RPALEN);
    ctoken_map_add(punctuator_map, "{", CTOKEN_LBRACE);
    ctoken_map_add(punctuator_map, "}", CTOKEN_RBRACE);
    ctoken_map_add(punctuator_map, ".", CTOKEN_DOT);
    ctoken_map_add(punctuator_map, "->", CTOKEN_ARROW);
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

Set* new_white_space_set(void) {
    Set* white_space_set = new_set(&t_hashable_integer);

    int NUM_WHITE_SPACES = 6;
    char spaces[6] = " \t\n\v\f";

    for (int i = 0; i < NUM_WHITE_SPACES; i++) {
        set_add(white_space_set, new_integer(spaces[i]));
    }

    return white_space_set;
}

IntegerLiteral* create_iliteral(char* value, int base, IntegerLiteralType type) {
    if (iliteral_type_isunsigned(type)) {
        unsigned long long unsigned_value = strtoull(value, NULL, base);
        return new_unsigned_iliteral(type, unsigned_value);
    } else {
        long long signed_value = strtoll(value, NULL, base);
        return new_signed_iliteral(type, signed_value);
    }
}

void skip_white_spaces(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    while (set_contains(lexer->white_space_set, &c)) {
        c = fgetc(lexer->file_ptr);
    }
    ungetc(c, lexer->file_ptr);
}

void integer_suffix_map_add(Map* integer_suffix_map, char* suffix_str, IntegerLiteralType type) {
    char* key = new_string(suffix_str);
    IntegerLiteralType* value = malloc(sizeof(IntegerLiteralType));
    *value = type;
    map_add(integer_suffix_map, key, value);
}

void ctoken_map_add(Map* ctoken_map, char* ctoken_str, CTokenType type) {
    char* key = new_string(ctoken_str);
    CTokenType* value = malloc(sizeof(CTokenType));
    *value = type;
    map_add(ctoken_map, key, value);
}
