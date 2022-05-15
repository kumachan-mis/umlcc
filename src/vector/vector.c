#include "./vector.h"
#include "../common/common.h"

#include <memory.h>
#include <stdlib.h>

struct _Vector {
    BaseType* t_item;
    void** container;
    int size;
    int capacity;
};

Vector* new_vector(BaseType* t_item) {
    Vector* vector = malloc(sizeof(Vector));
    int size = 0, capacity = 1;

    vector->t_item = t_item;
    vector->container = malloc(capacity * sizeof(void*));
    vector->size = size;
    vector->capacity = capacity;
    return vector;
}

void delete_vector(Vector* vector) {
    for (int i = 0; i < vector->size; i++) {
        vector->t_item->delete_object(vector->container[i]);
    }
    free(vector->container);
    free(vector);
}

Vector* vector_copy(Vector* vector) {
    Vector* copied_vector = malloc(sizeof(Vector));

    copied_vector->t_item = vector->t_item;
    copied_vector->container = malloc(vector->capacity * sizeof(void*));
    copied_vector->size = vector->size;
    copied_vector->capacity = vector->capacity;

    for (int i = 0; i < vector->size; i++) {
        copied_vector->container[i] = vector->t_item->copy_object(copied_vector->container[i]);
    }
    return copied_vector;
}

void vector_push(Vector* vector, void* item) {
    if (vector->size >= vector->capacity) {
        vector->container = realloc(vector->container, 2 * vector->capacity * sizeof(void*));
        vector->capacity *= 2;
    }
    vector->container[vector->size] = item;
    vector->size++;
}

void* vector_pop(Vector* vector) {
    void* item = vector->container[vector->size - 1];
    vector->size--;
    if (vector->capacity > 1 && 2 * vector->size < vector->capacity) {
        vector->container = realloc(vector->container, vector->capacity / 2 * sizeof(void*));
        vector->capacity /= 2;
    }
    return item;
}

void vector_fill(Vector* vector, int size, void* item) {
    if (size <= vector->size) {
        vector->t_item->delete_object(item);
        return;
    }

    if (vector->capacity < size) {
        vector->container = realloc(vector->container, size * sizeof(void*));
        vector->capacity = size;
    }
    for (int i = vector->size; i < size; i++) {
        vector->container[i] = vector->t_item->copy_object(item);
    }
    vector->size = size;
    vector->t_item->delete_object(item);
}

int vector_set(Vector* vector, int index, void* item) {
    if (0 > index || index >= vector->size) return 0;
    vector->t_item->delete_object(vector->container[index]);
    vector->container[index] = item;
    return 1;
}

void* vector_at(Vector* vector, int index) {
    if (0 > index || index >= vector->size) return NULL;
    return vector->container[index];
}

int vector_size(Vector* vector) {
    return vector->size;
}

void vector_extend(Vector* vector, Vector* other) {
    for (int i = 0; i < other->size; i++) {
        vector_push(vector, other->container[i]);
    }

    int size = 0, capacity = 1;
    other->container = realloc(other->container, capacity * sizeof(void*));
    other->size = size;
    other->capacity = capacity;
}
