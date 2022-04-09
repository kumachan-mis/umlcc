#ifndef _UMLCC_PARSER_H_
#define _UMLCC_PARSER_H_

#include "../vector/vector.h"


typedef struct _Parser Parser;

Parser* new_parser(Vector* tokens);
void    delete_parser(Parser* parser);
Vector* parser_create_asts(Parser* parser);

#endif
