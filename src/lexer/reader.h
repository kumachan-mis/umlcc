#ifndef UMLCC_LEXER_READER_H
#define UMLCC_LEXER_READER_H

#include "../lexer/lexer.h"

CToken* read_keyword_or_identifier(Lexer* lexer);
CToken* read_integer_constant(Lexer* lexer);
CToken* read_character_constant(Lexer* lexer);
CToken* read_string_literal(Lexer* lexer);
CToken* read_punctuator(Lexer* lexer);

#endif
