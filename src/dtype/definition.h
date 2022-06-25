#ifndef UMLCC_TYPE_DEFINITION_H
#define UMLCC_TYPE_DEFINITION_H

typedef struct DDefinition {
    struct Dtype* def_dtype;
} DDefinition;

DDefinition* new_ddefinition(struct Dtype* def_dtype);
DDefinition* new_socket_ddefinition();
DDefinition* ddefinition_copy(DDefinition* ddefinition);
struct Dtype* ddefinition_next(DDefinition* ddefinition);
DDefinition* ddefinition_connect(DDefinition* socket, struct Dtype* plug);
int ddefinition_equals(DDefinition* ddefinition, DDefinition* other);
void delete_ddefinition(DDefinition* ddefinition);

#endif
