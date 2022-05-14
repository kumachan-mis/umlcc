#ifndef _UMLCC_COMMON_H_
#define _UMLCC_COMMON_H_

int* new_integer(int value);
void delete_integer(int* integer);
int integer_hash(int* integer, int max);
int integer_comp(int* a, int* b);

char* new_string(char* str);
void delete_str(char* str);
int str_hash(char* str, int max);
int str_comp(char* a, char* b);

#endif
