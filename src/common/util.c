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

void* copy_memory_zero_padding(void* src, int src_size, int dst_size) {
    void* dst = malloc(dst_size);
    memset(dst, 0, dst_size);
    memcpy(dst, src, src_size);
    return dst;
}
