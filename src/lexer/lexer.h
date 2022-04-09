#ifndef _UMLCC_LEXER_H_
#define _UMLCC_LEXER_H_

#include "../token/token.h"

#include <stdio.h>


typedef struct _Lexer Lexer;

Lexer* new_lexer(FILE* file_ptr);
void   delete_lexer(Lexer* lexer);
Token* lexer_read_next_token(Lexer* lexer);

#endif
