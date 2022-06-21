#ifndef UMLCC_COMMON_UTIL_H
#define UMLCC_COMMON_UTIL_H

void swap_ptr(void** a, void** b);
void* copy_memory(void* src, int size);
void* copy_memory_zero_padding(void* src, int src_size, int dst_size);

#endif
