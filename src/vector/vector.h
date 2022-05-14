#ifndef _UMLCC_VECTOR_H_
#define _UMLCC_VECTOR_H_

typedef struct _Vector Vector;

#include "../common/common.h"

Vector* new_vector(BaseType* t_item);
void delete_vector(Vector* vector);
void vector_push(Vector* vector, void* item);
void* vector_pop(Vector* vector);
void* vector_at(Vector* vector, int index);
int vector_size(Vector* vector);
void vector_extend(Vector* vector, Vector* other);

#endif
