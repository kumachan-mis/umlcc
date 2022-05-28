#include "./array.h"

#include <stdlib.h>

struct Dtype* dtype_copy(struct Dtype* dtype);
void delete_dtype(struct Dtype* dtype);

DArray* new_darray(struct Dtype* of_dtype, int size) {
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

struct Dtype* darray_next(DArray* darray) {
    return darray->of_dtype;
}

DArray* darray_connect(DArray* socket, struct Dtype* plug) {
    socket->of_dtype = plug;
    return socket;
}

void delete_darray(DArray* darray) {
    if (darray->of_dtype != NULL) delete_dtype(darray->of_dtype);
    free(darray);
}
