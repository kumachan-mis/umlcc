#ifndef LEXER_DYSTRING_H
#define LEXER_DYSTRING_H

typedef struct Builder Builder;

Builder* new_builder();
void builder_push(Builder* builder, char c);
char* builder_build(Builder* builder);
void delete_builder(Builder* builder);

#endif
