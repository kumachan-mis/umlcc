#ifndef UMLCC_DTYPE_STRUCT_H
#define UMLCC_DTYPE_STRUCT_H

#include "../common/type.h"
#include "../vector/vector.h"

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;

DType* dtype_copy(DType* dtype);
int dtype_equals(DType* dtype, DType* other);
int dtype_nbytes(DType* dtype);
void delete_dtype(DType* dtype);
#endif

typedef struct DStruct DStruct;
typedef struct DMember DMember;

struct DStruct {
    char* name;
    Vector* members;
    int nbytes;
};

struct DMember {
    char* name;
    DType* dtype;
};

#ifdef UMLCC_DTYPE_H_PRIVATE
DStruct* new_named_dstruct(char* name, int nbytes);
DStruct* new_unnamed_dstruct(Vector* members);
DStruct* dstruct_copy(DStruct* dstruct);
DType* dstruct_at(DStruct* dstruct, int index);
int dstruct_size(DStruct* dstruct);
int dstruct_equals(DStruct* dstruct, DStruct* other);
void delete_dstruct(DStruct* dstruct);
#endif

extern BaseType t_dmember;

DMember* new_dmember(char* name, DType* dtype);
DMember* dmember_copy(DMember* dmember);
int dmember_equals(DMember* dmember, DMember* other);
void delete_dmember(DMember* dmember);

#endif
