#include "./decoration.h"

#include <stdlib.h>

struct Dtype* dtype_copy(struct Dtype* dtype);
void delete_dtype(struct Dtype* dtype);
int dtype_equals(struct Dtype* dtype, struct Dtype* other);

DDecoration* new_ddecoration(struct Dtype* deco_dtype) {
    DDecoration* ddecoration = malloc(sizeof(DDecoration));
    ddecoration->typedef_flag = 0;
    ddecoration->deco_dtype = deco_dtype;
    return ddecoration;
}

DDecoration* new_socket_ddecoration(void) {
    return new_ddecoration(NULL);
}

DDecoration* ddecoration_copy(DDecoration* ddecoration) {
    DDecoration* copied_ddecoration = malloc(sizeof(DDecoration));
    copied_ddecoration->typedef_flag = ddecoration->typedef_flag;
    copied_ddecoration->deco_dtype = dtype_copy(ddecoration->deco_dtype);
    return copied_ddecoration;
}

struct Dtype* ddecoration_next(DDecoration* ddecoration) {
    return ddecoration->deco_dtype;
}

DDecoration* ddecoration_connect(DDecoration* socket, struct Dtype* plug) {
    socket->deco_dtype = plug;
    return socket;
}

int ddecoration_equals(DDecoration* ddecoration, DDecoration* other) {
    if (ddecoration->typedef_flag != other->typedef_flag) return 0;
    return dtype_equals(ddecoration->deco_dtype, other->deco_dtype);
}

void delete_ddecoration(DDecoration* ddecoration) {
    delete_dtype(ddecoration->deco_dtype);
    free(ddecoration);
}
