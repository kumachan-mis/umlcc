#include "./parser.h"
#include "../ctoken/ctoken.h"
#include "./external.h"

#include <stdlib.h>

Parser* new_parser(Vector* ctokens) {
    Parser* parser = malloc(sizeof(Parser));
    parser->ctokens = ctokens;
    parser->index = 0;
    parser->typedef_names_set = new_set(&t_hashable_string);
    parser->typedef_flag = 0;
    return parser;
}

ParserReturn* parser_create_ast(Parser* parser) {
    return parse_translation_unit(parser);
}

void delete_parser(Parser* parser) {
    delete_vector(parser->ctokens);
    delete_set(parser->typedef_names_set);
    free(parser);
}
