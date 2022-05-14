#ifndef _UMLCC_COMMON_H_
#define _UMLCC_COMMON_H_

typedef struct _BaseType {
    void (*delete_object)(void* object);
} BaseType;

typedef struct _HashableType {
    int (*hash_object)(void* object);
    int (*compare_object)(void* a, void* b);
    void* (*copy_object)(void* object);
    void (*delete_object)(void* object);
} HashableType;

int* new_integer(int value);
char* new_string(char* str);

extern BaseType t_integer;
extern BaseType t_string;

extern HashableType t_hashable_integer;
extern HashableType t_hashable_string;

#endif
