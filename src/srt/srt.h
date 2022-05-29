#ifndef UMLCC_SRT_H
#define UMLCC_SRT_H

#include "../common/type.h"
#include "../dtype/dtype.h"
#include "../vector/vector.h"

typedef enum SrtType {
    // translation unit
    SRT_TRAS_UNIT,

    // function definition
    SRT_FUNC_DEF,

    // declaration
    SRT_DECL_LIST,
    SRT_INIT_DECL,
    SRT_DECL,

    // statement
    SRT_CMPD_STMT,
    SRT_RET_STMT,
    SRT_EXPR_STMT,

    // expression
    //   assignment-expression
    SRT_ASSIGN_EXPR,
    //   logical-OR-expression
    SRT_LOR_EXPR,
    //   logical-AND-expression
    SRT_LAND_EXPR,
    //   equality-expression
    SRT_EQUAL_EXPR,
    SRT_NEQUAL_EXPR,
    //   additive-expression
    SRT_ADD_EXPR,
    SRT_SUB_EXPR,
    SRT_PADD_EXPR,
    SRT_PSUB_EXPR,
    SRT_PDIFF_EXPR,
    //   multiplicative-expression
    SRT_MUL_EXPR,
    SRT_DIV_EXPR,
    SRT_MOD_EXPR,
    //   unary-expression
    SRT_ADDR_EXPR,
    SRT_INDIR_EXPR,
    SRT_LNOT_EXPR,
    //   postfix-expression
    SRT_CALL_EXPR,
    SRT_ARG_LIST,
    //   primary-expression
    SRT_IDENT_EXPR,
    SRT_INT_EXPR,
} SrtType;

typedef struct Srt {
    SrtType type;
    Dtype* dtype;
    char* ident_name;
    int value_int;
    Vector* children;
} Srt;

extern BaseType t_srt;

Srt* new_srt(SrtType type, int num_children, ...);
Srt* new_dtyped_srt(SrtType type, Dtype* dtype, int num_children, ...);
Srt* new_identifier_srt(SrtType type, Dtype* dtype, char* ident_name);
Srt* new_integer_srt(SrtType type, int value);
Srt* srt_copy(Srt* srt);
void delete_srt(Srt* srt);

#endif
