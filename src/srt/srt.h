#ifndef UMLCC_SRT_H
#define UMLCC_SRT_H

#include "../common/type.h"
#include "../dtype/dtype.h"
#include "../literal/iliteral.h"
#include "../literal/sliteral.h"
#include "../vector/vector.h"

typedef enum SrtType {
    // translation unit
    SRT_TRAS_UNIT,

    // function definition
    SRT_FUNC_DEF,

    // declaration
    SRT_DECL_LIST,
    SRT_TAG_DECL,
    SRT_INIT_DECL,
    SRT_DECL,
    SRT_STRDECL,
    SRT_INIT,

    // statement
    SRT_CMPD_STMT,
    SRT_RET_STMT,
    SRT_EXPR_STMT,
    SRT_IF_STMT,
    SRT_WHILE_STMT,

    // expression
    //   assignment-expression
    SRT_ASSIGN_EXPR,
    //  conditional-expression
    SRT_COND_EXPR,
    //   logical-OR-expression
    SRT_LOR_EXPR,
    //   logical-AND-expression
    SRT_LAND_EXPR,
    //   inclusive-OR-expression
    SRT_OR_EXPR,
    //   exclusive-OR-expression
    SRT_XOR_EXPR,
    //   AND-expression
    SRT_AND_EXPR,
    //   equality-expression
    SRT_EQUAL_EXPR,
    SRT_NEQUAL_EXPR,
    //   relational-expression
    SRT_LESS_EXPR,
    SRT_GREATER_EXPR,
    SRT_LESSEQ_EXPR,
    SRT_GREATEREQ_EXPR,
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
    //   cast-expression
    SRT_CAST_EXPR,
    //   unary-expression
    SRT_ADDR_EXPR,
    SRT_INDIR_EXPR,
    SRT_POS_EXPR,
    SRT_NEG_EXPR,
    SRT_NOT_EXPR,
    SRT_LNOT_EXPR,
    //   postfix-expression
    SRT_CALL_EXPR,
    SRT_TOMEMBER_EXPR,
    SRT_ARG_LIST,
    //   primary-expression
    SRT_IDENT_EXPR,
    SRT_STRIDENT_EXPR,
    SRT_INT_EXPR,
    SRT_CHAR_EXPR,
    SRT_STRING_EXPR,
} SrtType;

typedef struct Srt {
    SrtType type;
    DType* dtype;
    char* ident_name;
    int sliteral_id;
    IntegerLiteral* iliteral;
    StringLiteral* sliteral;
    Vector* children;
} Srt;

extern BaseType t_srt;

Srt* new_srt(SrtType type, int num_children, ...);
Srt* new_dtyped_srt(SrtType type, DType* dtype, int num_children, ...);
Srt* new_identifier_srt(SrtType type, DType* dtype, char* ident_name);
Srt* new_sliteral_identifier_srt(SrtType type, DType* dtype, int sliteral_id);
Srt* new_iliteral_srt(SrtType type, DType* dtype, IntegerLiteral* iliteral);
Srt* new_sliteral_srt(SrtType type, DType* dtype, StringLiteral* sliteral);
Srt* srt_copy(Srt* srt);
int srt_ismodifiable(Srt* srt);
void delete_srt(Srt* srt);

#endif
