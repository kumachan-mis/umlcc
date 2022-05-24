#include "./builder.h"

#include <stdlib.h>

struct _Builder {
    char* _data;
    int _length;
    int _capacity;
};

Builder* new_builder() {
    Builder* builder = malloc(sizeof(Builder));
    int length = 0, capacity = 1;

    builder->_data = malloc(sizeof(char) * capacity);
    builder->_data[0] = '\0';
    builder->_length = length;
    builder->_capacity = capacity;

    return builder;
}

void delete_builder(Builder* builder) {
    free(builder->_data);
    free(builder);
}

void builder_push(Builder* builder, char c) {
    builder->_data[builder->_length] = c;
    builder->_length++;
    if (builder->_length >= builder->_capacity) {
        builder->_data = realloc(builder->_data, 2 * builder->_capacity * sizeof(char));
        builder->_capacity *= 2;
    }
    builder->_data[builder->_length] = '\0';
}

char* builder_finish(Builder* builder) {
    char* data = builder->_data;
    data = realloc(data, (builder->_length + 1) * sizeof(char));
    free(builder);
    return data;
}
