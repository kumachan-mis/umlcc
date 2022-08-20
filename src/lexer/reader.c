#include "./reader.h"
#include "../literal/sliteral.h"

#include <stdlib.h>
#include <string.h>

LexerReturnItem* read_keyword_or_identifier(Lexer* lexer) {
    int length = 0, capacity = 4;
    char* ctoken_str = malloc(sizeof(char) * capacity);

    int c = fgetc(lexer->file_ptr);
    ctoken_str[length] = c;
    length++;

    while (1) {
        c = fgetc(lexer->file_ptr);
        if (!set_contains(lexer->nondigit_set, &c) && !map_contains(lexer->digit_map, &c)) {
            ungetc(c, lexer->file_ptr);
            break;
        }
        ctoken_str[length] = c;
        length++;
        if (length + 1 >= capacity) {
            ctoken_str = realloc(ctoken_str, 2 * capacity * sizeof(char));
            capacity *= 2;
        }
    }
    ctoken_str[length] = '\0';

    CTokenType* ctoken_type_ref = map_get(lexer->keyword_map, ctoken_str);
    if (ctoken_type_ref != NULL) {
        free(ctoken_str);
        CToken* ctoken = new_ctoken(*ctoken_type_ref);
        return new_lexerret_item(ctoken);
    }

    ctoken_str = realloc(ctoken_str, (length + 1) * sizeof(char));
    CToken* ctoken = new_identifier_ctoken(CTOKEN_IDENT, ctoken_str);
    return new_lexerret_item(ctoken);
}

LexerReturnItem* read_integer_constant(Lexer* lexer) {
    int read_decimal_constant(Lexer * lexer);
    int read_octal_constant(Lexer * lexer);
    int read_hexadecimal_constant(Lexer * lexer);

    int fst = fgetc(lexer->file_ptr);
    int snd = fgetc(lexer->file_ptr);

    if (fst == '0' && (snd == 'x' || snd == 'X')) {
        int value = read_hexadecimal_constant(lexer);
        CToken* ctoken = new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, value));
        return new_lexerret_item(ctoken);
    }

    ungetc(snd, lexer->file_ptr);
    ungetc(fst, lexer->file_ptr);

    if (fst == '0') {
        int value = read_octal_constant(lexer);
        CToken* ctoken = new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, value));
        return new_lexerret_item(ctoken);
    }

    int value = read_decimal_constant(lexer);
    CToken* ctoken = new_iliteral_ctoken(CTOKEN_INT, new_signed_iliteral(INTEGER_INT, value));
    return new_lexerret_item(ctoken);
}

LexerReturnItem* read_character_constant(Lexer* lexer) {
    int read_escape_seqence(Lexer * lexer);

    Error* err = NULL;

    fgetc(lexer->file_ptr);

    int c = fgetc(lexer->file_ptr);
    switch (c) {
        case '\\':
            c = read_escape_seqence(lexer);
            if (c != EOF) break;
            err = new_error("Error: invalid escape sequence\n");
            return new_lexerret_item_error(err);
        case '\n':
            err = new_error("Error: newline appeared in character constant\n");
            return new_lexerret_item_error(err);
        case '\'':
            err = new_error("Error: character constant is empty\n");
            return new_lexerret_item_error(err);
        default:
            break;
    }

    int terminated = 0;
    while (!terminated) {
        int rest = fgetc(lexer->file_ptr);
        switch (rest) {
            case '\\':
                rest = read_escape_seqence(lexer);
                if (rest != EOF) break;
                err = new_error("Error: invalid escape sequence\n");
                return new_lexerret_item_error(err);
            case '\n':
                err = new_error("Error: newline appeared in character constant\n");
                return new_lexerret_item_error(err);
            case '\'':
                terminated = 1;
                break;
            default:
                break;
        }
    }

    CToken* ctoken = new_iliteral_ctoken(CTOKEN_CHAR, new_signed_iliteral(INTEGER_INT, c));
    return new_lexerret_item(ctoken);
}

LexerReturnItem* read_string_literal(Lexer* lexer) {
    int read_escape_seqence(Lexer * lexer);

    Error* err = NULL;

    int length = 0, capacity = 4;
    char* value = malloc(sizeof(char) * capacity);

    fgetc(lexer->file_ptr);

    int terminated = 0;
    while (1) {
        int c = fgetc(lexer->file_ptr);
        switch (c) {
            case '\\':
                c = read_escape_seqence(lexer);
                if (c != EOF) break;
                free(value);
                err = new_error("Error: invalid escape sequence\n");
                return new_lexerret_item_error(err);
            case '\"':
                terminated = 1;
                break;
            case '\n':
                free(value);
                err = new_error("Error: newline appeared in string literal\n");
                return new_lexerret_item_error(err);
            default:
                break;
        }
        if (terminated) break;

        value[length] = c;
        length++;
        if (length + 1 >= capacity) {
            value = realloc(value, 2 * capacity * sizeof(char));
            capacity *= 2;
        }
    }
    value[length] = '\0';

    value = realloc(value, (length + 1) * sizeof(char));
    CToken* ctoken = new_sliteral_ctoken(CTOKEN_STRING, new_sliteral(value, length + 1));
    return new_lexerret_item(ctoken);
}

LexerReturnItem* read_punctuator(Lexer* lexer) {
    int MAX_PUNCUATOR_LEN = 3;

    Error* err = NULL;

    char* ctoken_str = malloc((MAX_PUNCUATOR_LEN + 1) * sizeof(int));
    int length = 0;
    memset(ctoken_str, 0, MAX_PUNCUATOR_LEN + 1);

    int c = fgetc(lexer->file_ptr);
    if (c == EOF) {
        free(ctoken_str);
        return new_lexerret_item(new_ctoken(CTOKEN_EOF));
    }
    ctoken_str[length] = c;
    length++;

    for (int i = 0; i < MAX_PUNCUATOR_LEN - 1; i++) {
        c = fgetc(lexer->file_ptr);
        if (c == EOF) {
            ungetc(c, lexer->file_ptr);
            break;
        }
        ctoken_str[length] = c;
        length++;
    }

    CTokenType* ctoken_type_ref = NULL;
    while (length > 0) {
        ctoken_type_ref = map_get(lexer->punctuator_map, ctoken_str);
        if (ctoken_type_ref != NULL) break;

        length--;
        c = ctoken_str[length];
        ungetc(ctoken_str[length], lexer->file_ptr);
        ctoken_str[length] = '\0';
    }
    free(ctoken_str);

    if (ctoken_type_ref == NULL) {
        err = new_error("Error: unexpected character '%c'\n", c);
        return new_lexerret_item_error(err);
    }

    CToken* ctoken = new_ctoken(*ctoken_type_ref);
    return new_lexerret_item(ctoken);
}

int read_decimal_constant(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    int* digit_ref = map_get(lexer->digit_map, &c);
    int value = *digit_ref;

    while (1) {
        c = fgetc(lexer->file_ptr);
        digit_ref = map_get(lexer->digit_map, &c);
        if (digit_ref != NULL) {
            value = value * 10 + *digit_ref;
            continue;
        }
        ungetc(c, lexer->file_ptr);
        break;
    }

    return value;
}

int read_octal_constant(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    int* octdigit_ref = map_get(lexer->octdigit_map, &c);
    int value = *octdigit_ref;

    while (1) {
        c = fgetc(lexer->file_ptr);
        octdigit_ref = map_get(lexer->octdigit_map, &c);
        if (octdigit_ref != NULL) {
            value = value * 010 + *octdigit_ref;
            continue;
        }
        ungetc(c, lexer->file_ptr);
        break;
    }

    return value;
}

int read_hexadecimal_constant(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    int* hexdigit_ref = map_get(lexer->hexdigit_map, &c);
    int value = *hexdigit_ref;

    while (1) {
        c = fgetc(lexer->file_ptr);
        hexdigit_ref = map_get(lexer->hexdigit_map, &c);
        if (hexdigit_ref != NULL) {
            value = value * 0x10 + *hexdigit_ref;
            continue;
        }
        ungetc(c, lexer->file_ptr);
        break;
    }

    return value;
}

int read_escape_seqence(Lexer* lexer) {
    int read_octal_escape_seqence(Lexer * lexer);
    int read_hexadecimal_escape_seqence(Lexer * lexer);
    int read_simple_escape_seqence(Lexer * lexer);

    int c = fgetc(lexer->file_ptr);
    if (c == 'x') return read_hexadecimal_escape_seqence(lexer);

    ungetc(c, lexer->file_ptr);
    if (map_contains(lexer->octdigit_map, &c)) return read_octal_escape_seqence(lexer);

    return read_simple_escape_seqence(lexer);
}

int read_octal_escape_seqence(Lexer* lexer) {
    int MAX_OCTAL_SEQUENCE_LEN = 3;

    int c = fgetc(lexer->file_ptr);
    int* octdigit_ref = map_get(lexer->octdigit_map, &c);
    int value = *octdigit_ref;

    for (int i = 0; i < MAX_OCTAL_SEQUENCE_LEN - 1; i++) {
        c = fgetc(lexer->file_ptr);
        octdigit_ref = map_get(lexer->octdigit_map, &c);
        if (octdigit_ref != NULL) {
            value = value * 010 + *octdigit_ref;
            continue;
        }
        ungetc(c, lexer->file_ptr);
        break;
    }

    return value;
}

int read_hexadecimal_escape_seqence(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    int* hexdigit_ref = map_get(lexer->hexdigit_map, &c);
    int value = *hexdigit_ref;

    while (1) {
        c = fgetc(lexer->file_ptr);
        hexdigit_ref = map_get(lexer->hexdigit_map, &c);
        if (hexdigit_ref != NULL) {
            value = value * 0x10 + *hexdigit_ref;
            continue;
        }
        ungetc(c, lexer->file_ptr);
        break;
    }

    return value;
}

int read_simple_escape_seqence(Lexer* lexer) {
    int c = fgetc(lexer->file_ptr);
    switch (c) {
        case '\'':
            return '\'';
        case '\"':
            return '\"';
        case '\?':
            return '\?';
        case '\\':
            return '\\';
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        default:
            return EOF;
    }
}
