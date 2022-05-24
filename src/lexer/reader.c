#include "./reader.h"
#include "./builder.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

CToken* read_keyword_or_identifier(Lexer* lexer) {
    Builder* builder = new_builder();

    char c = fgetc(lexer->file_ptr);
    if (!isalpha(c) && c != '_') {
        ungetc(c, lexer->file_ptr);
        delete_builder(builder);
        return NULL;
    }

    builder_push(builder, c);
    while (1) {
        c = fgetc(lexer->file_ptr);
        if (!isalpha(c) && !isdigit(c) && c != '_') {
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

    char c = fgetc(lexer->file_ptr);
    if (!isdigit(c)) {
        ungetc(c, lexer->file_ptr);
        delete_builder(builder);
        return NULL;
    }

    builder_push(builder, c);
    while (1) {
        c = fgetc(lexer->file_ptr);
        if (!isdigit(c)) {
            ungetc(c, lexer->file_ptr);
            break;
        }
        builder_push(builder, c);
    }

    char* ctoken_str = builder_build(builder);
    CToken* ctoken = new_integer_ctoken(atoi(ctoken_str));
    free(ctoken_str);

    return ctoken;
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
        ungetc(ctoken_str[length], lexer->file_ptr);
        ctoken_str[length] = '\0';
    }

    free(ctoken_str);
    return NULL;
}
