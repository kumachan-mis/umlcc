#ifndef UMLCC_DTYPE_STRUCT_H
#define UMLCC_DTYPE_STRUCT_H

#include "../common/type.h"
#include "../vector/vector.h"

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;

DType* dtype_copy(DType* dtype);
int dtype_equals(DType* dtype, DType* other);
int dtype_alignment(DType* dtype);
int dtype_nbytes(DType* dtype);
void delete_dtype(DType* dtype);
#endif

typedef struct DStruct DStruct;
typedef struct DStructMember DStructMember;

struct DStruct {
    char* name;
    Vector* members;
    int nbytes;
    int alignment;
};

struct DStructMember {
    char* name;
    DType* dtype;
    int memory_offset;
};

#ifdef UMLCC_DTYPE_H_PRIVATE
DStruct* new_named_dstruct(char* name, int nbytes, int alignment);
DStruct* new_unnamed_dstruct(Vector* members);
DStruct* dstruct_copy(DStruct* dstruct);
DType* dstruct_at(DStruct* dstruct, int index);
int dstruct_size(DStruct* dstruct);
int dstruct_equals(DStruct* dstruct, DStruct* other);
void delete_dstruct(DStruct* dstruct);
#endif

extern BaseType t_dstructmember;

DStructMember* new_dstructmember(char* name, DType* dtype);
DStructMember* dstructmember_copy(DStructMember* dstructmember);
int dstructmember_equals(DStructMember* dstructmember, DStructMember* other);
void delete_dstructmember(DStructMember* dstructmember);

#endif
