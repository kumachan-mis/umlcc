#ifndef UMLCC_DTYPE_ARRAY_H
#define UMLCC_DTYPE_ARRAY_H

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;

DType* dtype_copy(DType* dtype);
int dtype_equals(DType* dtype, DType* other);
void delete_dtype(DType* dtype);
#endif

typedef struct DArray DArray;

struct DArray {
    DType* of_dtype;
    int size;
};

#ifdef UMLCC_DTYPE_H_PRIVATE
DArray* new_darray(DType* of_dtype, int size);
DArray* new_socket_darray(int size);
DArray* darray_copy(DArray* darray);
DType* darray_next(DArray* darray);
DArray* darray_connect(DArray* socket, DType* plug);
DType* darray_at(DArray* darray, int index);
int darray_size(DArray* darray);
int darray_equals(DArray* darray, DArray* other);
void delete_darray(DArray* darray);
#endif

#endif
