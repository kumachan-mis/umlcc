#include "./util.h"

void swap_ptr(void** a, void** b) {
    void* tmp = *a;
    *a = *b;
    *b = tmp;
}
