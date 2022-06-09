#include "./reader.h"
#include "./builder.h"

#include <stdlib.h>
#include <string.h>

CToken* read_keyword_or_identifier(Lexer* lexer) {
    Builder* builder = new_builder();

    int c = fgetc(lexer->file_ptr);
    builder_push(builder, c);

    while (1) {
        c = fgetc(lexer->file_ptr);
        if (!set_contains(lexer->nondigit_set, &c) && !map_contains(lexer->digit_map, &c)) {
            ungetc(c, lexer->file_ptr);
            break;
        }
        builder_push(builder, c);
    }

    char* ctoken_str = builder_build(builder);
    CTokenType* ctoken_ref = map_get(lexer->keyword_map, ctoken_str);

    if (ctoken_ref != NULL) {
        free(ctoken_str);
        return new_ctoken(*ctoken_ref);
    }

    return new_identifier_ctoken(ctoken_str);
}

CToken* read_integer_constant(Lexer* lexer) {
    Builder* builder = new_builder();

    int c = fgetc(lexer->file_ptr);
    builder_push(builder, c);

    while (1) {
        c = fgetc(lexer->file_ptr);
        if (!map_contains(lexer->digit_map, &c)) {
            ungetc(c, lexer->file_ptr);
            break;
        }
        builder_push(builder, c);
    }

    char* ctoken_str = builder_build(builder);
    CToken* ctoken = new_integer_ctoken(CTOKEN_INT, atoi(ctoken_str));
    free(ctoken_str);

    return ctoken;
}

CToken* read_character_constant(Lexer* lexer) {
    int read_escaped_character(Lexer * lexer);

    fgetc(lexer->file_ptr);

    int c = fgetc(lexer->file_ptr);
    switch (c) {
        case '\\':
            c = read_escaped_character(lexer);
            break;
        case '\'':
        case '\n':
            fprintf(stderr, "Error: unexpected character \\n\n");
            exit(1);
        default:
            break;
    }

    int terminated = 0;
    while (!terminated) {
        int rest = fgetc(lexer->file_ptr);
        switch (rest) {
            case '\\':
                rest = read_escaped_character(lexer);
                break;
            case '\'':
                terminated = 1;
                break;
            case '\n':
                fprintf(stderr, "Error: unexpected character \\n\n");
                exit(1);
            default:
                break;
        }
    }

    return new_integer_ctoken(CTOKEN_CHAR, c);
}

int read_escaped_character(Lexer* lexer) {
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
        case '0':
            // TODO: support octorial escape sequence
            return '\0';
        default:
            fprintf(stderr, "Error: unexpected character %c\n", c);
            exit(1);
    }
}

CToken* read_punctuator(Lexer* lexer) {
    int MAX_PUNCUATOR_LEN = 3;

    char* ctoken_str = malloc((MAX_PUNCUATOR_LEN + 1) * sizeof(int));
    int length = 0;
    memset(ctoken_str, 0, MAX_PUNCUATOR_LEN + 1);

    int c = fgetc(lexer->file_ptr);
    if (c == EOF) {
        free(ctoken_str);
        return new_ctoken(CTOKEN_EOF);
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

    while (length > 0) {
        CTokenType* ctoken_ref = map_get(lexer->punctuator_map, ctoken_str);
        if (ctoken_ref != NULL) {
            free(ctoken_str);
            return new_ctoken(*ctoken_ref);
        }
        length--;
        c = ctoken_str[length];
        ungetc(ctoken_str[length], lexer->file_ptr);
        ctoken_str[length] = '\0';
    }

    fprintf(stderr, "Error: unexpected character %c\n", c);
    exit(1);
}
