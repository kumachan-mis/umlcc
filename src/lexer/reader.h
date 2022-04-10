#ifndef _UMLCC_LEXER_READER_H_
#define _UMLCC_LEXER_READER_H_

#include "../lexer/lexer.h"


Token* read_identifier(Lexer* lexer);
Token* read_integer_constant(Lexer* lexer);
Token* read_punctuator(Lexer* lexer);

#endif
