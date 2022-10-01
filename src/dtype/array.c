#include "./array.h"

#include <stdlib.h>

DType* dtype_copy(DType* dtype);
void delete_dtype(DType* dtype);
int dtype_equals(DType* dtype, DType* other);

DArray* new_darray(DType* of_dtype, int size) {
    DArray* darray = malloc(sizeof(DArray));
    darray->of_dtype = of_dtype;
    darray->size = size;
    return darray;
}

DArray* new_socket_darray(int size) {
    return new_darray(NULL, size);
}

DArray* darray_copy(DArray* darray) {
    DArray* copied_darray = malloc(sizeof(DArray));
    copied_darray->of_dtype = NULL;
    if (darray->of_dtype != NULL) copied_darray->of_dtype = dtype_copy(darray->of_dtype);
    copied_darray->size = darray->size;
    return copied_darray;
}

DType* darray_next(DArray* darray) {
    return darray->of_dtype;
}

DArray* darray_connect(DArray* socket, DType* plug) {
    socket->of_dtype = plug;
    return socket;
}

int darray_equals(DArray* darray, DArray* other) {
    return darray->size == other->size && dtype_equals(darray->of_dtype, other->of_dtype);
}

void delete_darray(DArray* darray) {
    if (darray->of_dtype != NULL) delete_dtype(darray->of_dtype);
    free(darray);
}
