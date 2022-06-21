#ifndef UMLCC_X64_DATA_H
#define UMLCC_X64_DATA_H

typedef enum X64DataType {
    X64_DATA_BYTE,
    X64_DATA_WORD,
    X64_DATA_LONG,
    X64_DATA_QUAD,
    X64_DATA_ZERO,
    X64_DATA_STR,
} X64DataType;

typedef struct X64Data {
    X64DataType type;
    int imm_value;
    char* str_value;
    int str_size;
} X64Data;

X64Data* new_imm_x64data(X64DataType type, int value);
X64Data* new_str_x64data(X64DataType type, char* value, int size);
X64Data* x64data_copy(X64Data* x64data);
char* x64data_tostring(X64Data* x64data);
void delete_x64data(X64Data* x64data);
X64DataType x64data_get_type(int size);

#endif
