#ifndef UMLCC_TYPE_ARRAY_H
#define UMLCC_TYPE_ARRAY_H

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;
#endif

typedef struct DArray DArray;

struct DArray {
    DType* of_dtype;
    int size;
};

DArray* new_darray(DType* of_dtype, int size);
DArray* new_socket_darray(int size);
DArray* darray_copy(DArray* darray);
DType* darray_next(DArray* darray);
DArray* darray_connect(DArray* socket, DType* plug);
int darray_equals(DArray* darray, DArray* other);
void delete_darray(DArray* darray);

#endif
