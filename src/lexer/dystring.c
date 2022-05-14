#include "./dystring.h"

#include <stdlib.h>

struct _DyString {
    char* _data;
    int _length;
    int _capacity;
};

DyString* new_dystring() {
    DyString* dystring = malloc(sizeof(DyString));
    int length = 0, capacity = 1;

    dystring->_data = malloc(sizeof(char) * capacity);
    dystring->_data[0] = '\0';
    dystring->_length = length;
    dystring->_capacity = capacity;

    return dystring;
}

void delete_dystring(DyString* dystring) {
    free(dystring->_data);
    free(dystring);
}

void dystring_push(DyString* dystring, char c) {
    dystring->_data[dystring->_length] = c;
    dystring->_length++;
    if (dystring->_length >= dystring->_capacity) {
        dystring->_data = realloc(dystring->_data, 2 * dystring->_capacity * sizeof(char));
        dystring->_capacity *= 2;
    }
    dystring->_data[dystring->_length] = '\0';
}

char* dystring_finish(DyString* dystring) {
    char* data = dystring->_data;
    data = realloc(data, (dystring->_length + 1) * sizeof(char));
    free(dystring);
    return data;
}
