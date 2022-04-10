#include "./util.h"

#include <stdio.h>
#include <stdlib.h>


void consume_token(Parser* parser, TokenType token_type) {
    Token* token = vector_at(parser->_tokens, parser->_index);
    if (token->type == token_type) {
        parser->_index++;
        return;
    }
    fprintf(stderr, "Error: unexpected token type %d\n", token->type);
    exit(1);
}
