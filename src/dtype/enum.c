#ifndef UMLCC_DTYPE_H_PRIVATE
#define UMLCC_DTYPE_H_PRIVATE
#endif

#include "./enum.h"

#include <stdlib.h>
#include <string.h>

DEnum* new_named_denum(char* name) {
    DEnum* denum = malloc(sizeof(DEnum));
    denum->name = name;
    denum->members = NULL;
    return denum;
}

DEnum* new_unnamed_denum(Vector* members) {
    DEnum* denum = malloc(sizeof(DEnum));
    denum->name = NULL;
    denum->members = members;
    return denum;
}

DEnum* denum_copy(DEnum* denum) {
    DEnum* copied_denum = malloc(sizeof(DEnum));
    copied_denum->name = NULL;
    if (denum->name != NULL) copied_denum->name = new_string(denum->name);
    copied_denum->members = NULL;
    if (denum->members != NULL) copied_denum->members = vector_copy(denum->members);
    return copied_denum;
}

int denum_equals(DEnum* denum, DEnum* other) {
    if ((denum->name == NULL && other->name != NULL) || (denum->name != NULL && other->name == NULL)) return 0;

    // named enum
    if (denum->name != NULL) return strcmp(denum->name, other->name) == 0;

    // unnamed enum
    if (vector_size(denum->members) != vector_size(other->members)) return 0;

    int num_members = vector_size(denum->members);
    for (int i = 0; i < num_members; i++) {
        DEnumMember* denum_member = vector_at(denum->members, i);
        DEnumMember* other_member = vector_at(other->members, i);
        if (!denummember_equals(denum_member, other_member)) return 0;
    }
    return 1;
}

void delete_denum(DEnum* denum) {
    if (denum->name != NULL) free(denum->name);
    if (denum->members != NULL) delete_vector(denum->members);
}

DEnumMember* new_denummember(char* name, int value) {
    DEnumMember* denummember = malloc(sizeof(DEnumMember));
    denummember->name = name;
    denummember->value = value;
    return denummember;
}

DEnumMember* denummember_copy(DEnumMember* denummember) {
    DEnumMember* copied_denummember = malloc(sizeof(DEnumMember));
    copied_denummember->name = new_string(denummember->name);
    copied_denummember->value = denummember->value;
    return copied_denummember;
}

int denummember_equals(DEnumMember* denummember, DEnumMember* other) {
    return strcmp(denummember->name, other->name) == 0 && denummember->value == other->value;
}

void delete_denummember(DEnumMember* denummember) {
    free(denummember->name);
    free(denummember);
}
