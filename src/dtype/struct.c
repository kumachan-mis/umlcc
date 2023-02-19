#ifndef UMLCC_DTYPE_H_PRIVATE
#define UMLCC_DTYPE_H_PRIVATE
#endif

#include "./struct.h"
#include "../common/type.h"

#include <stdlib.h>
#include <string.h>

BaseType t_dmember = {
    .copy_object = (void* (*)(void*))dmember_copy,
    .delete_object = (void (*)(void*))delete_dmember,
};

DStruct* new_named_dstruct(char* name, int nbytes, int alignment) {
    DStruct* dstruct = malloc(sizeof(DStruct));
    dstruct->name = name;
    dstruct->members = NULL;
    dstruct->nbytes = nbytes;
    dstruct->alignment = alignment;
    return dstruct;
}

DStruct* new_unnamed_dstruct(Vector* members) {
    DStruct* dstruct = malloc(sizeof(DStruct));
    dstruct->name = NULL;
    dstruct->members = members;
    dstruct->nbytes = 0;
    dstruct->alignment = 0;

    int num_members = vector_size(members);
    for (int i = 0; i < num_members; i++) {
        DMember* member = vector_at(members, i);
        int alignment = dtype_alignment(member->dtype);

        member->memory_offset = (dstruct->nbytes + alignment - 1) / alignment * alignment;
        dstruct->nbytes = member->memory_offset;
        dstruct->nbytes += dtype_nbytes(member->dtype);

        if (alignment > dstruct->alignment) dstruct->alignment = alignment;
    }

    int nbytes = dstruct->nbytes, alignment = dstruct->alignment;
    dstruct->nbytes = (nbytes + alignment - 1) / alignment * alignment;
    return dstruct;
}

DStruct* dstruct_copy(DStruct* dstruct) {
    DStruct* copied_dstruct = malloc(sizeof(DStruct));
    copied_dstruct->name = NULL;
    if (dstruct->name != NULL) copied_dstruct->name = new_string(dstruct->name);
    copied_dstruct->members = NULL;
    if (dstruct->members != NULL) copied_dstruct->members = vector_copy(dstruct->members);
    copied_dstruct->nbytes = dstruct->nbytes;
    copied_dstruct->alignment = dstruct->alignment;
    return copied_dstruct;
}

DType* dstruct_at(DStruct* dstruct, int index) {
    DMember* member = vector_at(dstruct->members, index);
    if (member == NULL) return NULL;
    return member->dtype;
}

int dstruct_size(DStruct* dstruct) {
    return vector_size(dstruct->members);
}

int dstruct_equals(DStruct* dstruct, DStruct* other) {
    if ((dstruct->name == NULL && other->name != NULL) || (dstruct->name != NULL && other->name == NULL)) return 0;

    // named struct
    if (dstruct->name != NULL) return strcmp(dstruct->name, other->name) == 0;

    // unnamed struct
    if (vector_size(dstruct->members) != vector_size(other->members)) return 0;

    int num_members = vector_size(dstruct->members);
    for (int i = 0; i < num_members; i++) {
        DMember* dstruct_dmember = vector_at(dstruct->members, i);
        DMember* other_dmember = vector_at(other->members, i);
        if (!dmember_equals(dstruct_dmember, other_dmember)) return 0;
    }
    return 1;
}

void delete_dstruct(DStruct* dstruct) {
    if (dstruct->name != NULL) free(dstruct->name);
    if (dstruct->members != NULL) delete_vector(dstruct->members);
    free(dstruct);
}

DMember* new_dmember(char* name, DType* dtype) {
    DMember* dmember = malloc(sizeof(DMember));
    dmember->name = name;
    dmember->dtype = dtype;
    dmember->memory_offset = 0;
    return dmember;
}

DMember* dmember_copy(DMember* dmember) {
    DMember* copied_dmember = malloc(sizeof(DMember));
    copied_dmember->name = new_string(dmember->name);
    copied_dmember->dtype = dtype_copy(dmember->dtype);
    copied_dmember->memory_offset = dmember->memory_offset;
    return copied_dmember;
}

int dmember_equals(DMember* dmember, DMember* other) {
    return strcmp(dmember->name, other->name) == 0 && dtype_equals(dmember->dtype, other->dtype);
}

void delete_dmember(DMember* dmember) {
    free(dmember->name);
    delete_dtype(dmember->dtype);
    free(dmember);
}
