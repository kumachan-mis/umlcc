#include "./util.h"

#include <stdlib.h>
#include <string.h>

void swap_ptr(void** a, void** b) {
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}

void* copy_memory(void* src, int size) {
    void* dst = malloc(size);
    memcpy(dst, src, size);
    return dst;
}
