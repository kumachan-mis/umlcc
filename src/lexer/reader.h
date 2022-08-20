#ifndef UMLCC_LEXER_READER_H
#define UMLCC_LEXER_READER_H

#include "../lexer/lexer.h"

LexerReturnItem* read_keyword_or_identifier(Lexer* lexer);
LexerReturnItem* read_integer_constant(Lexer* lexer);
LexerReturnItem* read_character_constant(Lexer* lexer);
LexerReturnItem* read_string_literal(Lexer* lexer);
LexerReturnItem* read_punctuator(Lexer* lexer);

#endif
