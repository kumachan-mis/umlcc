#ifndef _LEXER_DYSTRING_H_
#define _LEXER_DYSTRING_H_

typedef struct _DyString DyString;

DyString* new_dystring();
void delete_dystring(DyString* dystring);
void dystring_push(DyString* dystring, char c);
char* dystring_finish(DyString* dystring);

#endif
