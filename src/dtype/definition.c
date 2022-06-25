#include "./definition.h"

#include <stdlib.h>

struct Dtype* dtype_copy(struct Dtype* dtype);
void delete_dtype(struct Dtype* dtype);
int dtype_equals(struct Dtype* dtype, struct Dtype* other);

DDefinition* new_ddefinition(struct Dtype* def_dtype) {
    DDefinition* ddefinition = malloc(sizeof(DDefinition));
    ddefinition->def_dtype = def_dtype;
    return ddefinition;
}

DDefinition* new_socket_ddefinition() {
    return new_ddefinition(NULL);
}

DDefinition* ddefinition_copy(DDefinition* ddefinition) {
    DDefinition* copied_ddefinition = malloc(sizeof(DDefinition));
    copied_ddefinition->def_dtype = dtype_copy(ddefinition->def_dtype);
    return copied_ddefinition;
}

struct Dtype* ddefinition_next(DDefinition* ddefinition) {
    return ddefinition->def_dtype;
}

DDefinition* ddefinition_connect(DDefinition* socket, struct Dtype* plug) {
    socket->def_dtype = plug;
    return socket;
}

int ddefinition_equals(DDefinition* ddefinition, DDefinition* other) {
    return dtype_equals(ddefinition->def_dtype, other->def_dtype);
}

void delete_ddefinition(DDefinition* ddefinition) {
    delete_dtype(ddefinition->def_dtype);
    free(ddefinition);
}
