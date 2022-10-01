#ifndef UMLCC_TYPE_ARRAY_H
#define UMLCC_TYPE_ARRAY_H

typedef struct DArray {
    struct DType* of_dtype;
    int size;
} DArray;

DArray* new_darray(struct DType* of_dtype, int size);
DArray* new_socket_darray(int size);
DArray* darray_copy(DArray* darray);
struct DType* darray_next(DArray* darray);
DArray* darray_connect(DArray* socket, struct DType* plug);
int darray_equals(DArray* darray, DArray* other);
void delete_darray(DArray* darray);

#endif
