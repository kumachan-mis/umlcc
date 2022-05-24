#ifndef _LEXER_DYSTRING_H_
#define _LEXER_DYSTRING_H_

typedef struct _Builder Builder;

Builder* new_builder();
void delete_builder(Builder* builder);
void builder_push(Builder* builder, char c);
char* builder_build(Builder* builder);

#endif
