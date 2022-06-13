#ifndef UMLCC_X64_H
#define UMLCC_X64_H

#include "../common/type.h"
#include "./data.h"
#include "./inst.h"
#include "./label.h"
#include "./register.h"

typedef enum X64Type {
    X64_INST,
    X64_DATA,
    X64_LABEL,
} X64Type;

typedef struct X64 {
    X64Type type;
    X64Inst* inst;
    X64Data* data;
    X64Label* label;
} X64;

extern BaseType t_x64;

X64* new_inst_x64(X64InstType type, X64Ope* src, X64Ope* dst);
X64* new_data_x64(X64DataType type, int value);
X64* new_label_x64(X64LabelType type, X64Visibility visibility, char* name);
X64* x64_copy(X64* x64);
char* x64_tostring(X64* x64);
void delete_x64(X64* x64);

#endif
