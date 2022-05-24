#include "./builder.h"

#include <stdlib.h>

struct Builder {
    char* data;
    int length;
    int capacity;
};

Builder* new_builder() {
    Builder* builder = malloc(sizeof(Builder));
    int length = 0, capacity = 1;

    builder->data = malloc(sizeof(char) * capacity);
    builder->data[0] = '\0';
    builder->length = length;
    builder->capacity = capacity;

    return builder;
}

void delete_builder(Builder* builder) {
    free(builder->data);
    free(builder);
}

void builder_push(Builder* builder, char c) {
    builder->data[builder->length] = c;
    builder->length++;
    if (builder->length >= builder->capacity) {
        builder->data = realloc(builder->data, 2 * builder->capacity * sizeof(char));
        builder->capacity *= 2;
    }
    builder->data[builder->length] = '\0';
}

char* builder_build(Builder* builder) {
    char* data = builder->data;
    data = realloc(data, (builder->length + 1) * sizeof(char));
    free(builder);
    return data;
}
