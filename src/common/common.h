#ifndef UMLCC_COMMON_H
#define UMLCC_COMMON_H

typedef struct BaseType {
    void* (*copy_object)(void* object);
    void (*delete_object)(void* object);
} BaseType;

typedef struct HashableType {
    void* (*copy_object)(void* object);
    void (*delete_object)(void* object);
    int (*hash_object)(void* object);
    int (*compare_object)(void* a, void* b);
} HashableType;

extern BaseType t_integer;
extern BaseType t_string;

extern HashableType t_hashable_integer;
extern HashableType t_hashable_string;

int* new_integer(int value);
char* new_string(char* str);
void swap_ptr(void** a, void** b);

#endif
