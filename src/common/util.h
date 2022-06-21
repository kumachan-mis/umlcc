#ifndef UMLCC_COMMON_UTIL_H
#define UMLCC_COMMON_UTIL_H

void swap_ptr(void** a, void** b);
char* copy_charmem(char* charmem, int size);
char* copy_charmem_zero_padding(char* charmem, int charmem_size, int copy_size);
char* charmem_tostring(char* charmem, int size);

#endif
