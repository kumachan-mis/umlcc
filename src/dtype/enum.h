#ifndef UMLCC_DTYPE_ENUM_H
#define UMLCC_DTYPE_ENUM_H

#include "../common/type.h"
#include "../vector/vector.h"

typedef struct DEnum DEnum;
typedef struct DEnumMember DEnumMember;

struct DEnum {
    char* name;
    Vector* members;
};

struct DEnumMember {
    char* name;
    int value;
};

#ifdef UMLCC_DTYPE_H_PRIVATE
DEnum* new_named_denum(char* name);
DEnum* new_unnamed_denum(Vector* members);
DEnum* denum_copy(DEnum* denum);
int denum_equals(DEnum* denum, DEnum* other);
void delete_denum(DEnum* denum);
#endif

extern BaseType t_denummember;

DEnumMember* new_denummember(char* name, int value);
DEnumMember* denummember_copy(DEnumMember* denummember);
int denummember_equals(DEnumMember* denummember, DEnumMember* other);
void delete_denummember(DEnumMember* denummember);

#endif
