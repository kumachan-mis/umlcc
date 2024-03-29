#ifndef UMLCC_VECTOR_H
#define UMLCC_VECTOR_H

typedef struct Vector Vector;

#include "../common/type.h"

Vector* new_vector(BaseType* t_item);
Vector* vector_copy(Vector* vector);
void vector_push(Vector* vector, void* item);
void* vector_pop(Vector* vector);
void vector_fill(Vector* vector, int size, void* item);
int vector_set(Vector* vector, int index, void* item);
void vector_insert(Vector* vector, int index, void* item);
void vector_erase(Vector* vector, int index);
void* vector_at(Vector* vector, int index);
int vector_size(Vector* vector);
void vector_extend(Vector* vector, Vector* other);
void delete_vector(Vector* vector);

#endif
