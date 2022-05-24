#include "./reader.h"
#include "./builder.h"

#include <dtype.h>
#include <stdlib.h>
#include <string.h>

Token* read_keyword_or_identifier(Lexer* lexer) {
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

    char* token_str = builder_build(builder);
    TokenType* token_ref = map_get(lexer->keyword_map, token_str);

    if (token_ref != NULL) {
        free(token_str);
        return new_token(*token_ref);
    }
    return new_identifier_token(token_str);
}

Token* read_integer_constant(Lexer* lexer) {
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

    char* token_str = builder_build(builder);
    Token* token = new_integer_token(atoi(token_str));
    free(token_str);

    return token;
}

Token* read_punctuator(Lexer* lexer) {
    int MAX_PUNCUATOR_LEN = 3;

    char* token_str = malloc((MAX_PUNCUATOR_LEN + 1) * sizeof(int));
    int length = 0;
    memset(token_str, 0, MAX_PUNCUATOR_LEN + 1);

    int c = fgetc(lexer->file_ptr);
    if (c == EOF) {
        free(token_str);
        return new_token(TOKEN_EOF);
    }
    token_str[length] = c;
    length++;

    for (int i = 0; i < MAX_PUNCUATOR_LEN - 1; i++) {
        c = fgetc(lexer->file_ptr);
        if (c == EOF) {
            ungetc(c, lexer->file_ptr);
            break;
        }
        token_str[length] = c;
        length++;
    }

    while (length > 0) {
        TokenType* token_ref = map_get(lexer->punctuator_map, token_str);
        if (token_ref != NULL) {
            free(token_str);
            return new_token(*token_ref);
        }
        length--;
        ungetc(token_str[length], lexer->file_ptr);
        token_str[length] = '\0';
    }

    free(token_str);
    return NULL;
}
