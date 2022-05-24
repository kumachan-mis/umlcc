#ifndef UMLCC_LEXER_READER_H
#define UMLCC_LEXER_READER_H

#include "../lexer/lexer.h"

Token* read_keyword_or_identifier(Lexer* lexer);
Token* read_integer_constant(Lexer* lexer);
Token* read_punctuator(Lexer* lexer);

#endif
