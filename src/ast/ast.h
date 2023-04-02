#ifndef UMLCC_AST_H
#define UMLCC_AST_H

#include "../common/type.h"
#include "../literal/iliteral.h"
#include "../literal/sliteral.h"
#include "../vector/vector.h"

typedef enum AstType {
    // translation unit
    AST_TRAS_UNIT,

    // function definition
    AST_FUNC_DEF,

    // declaration
    AST_DECL,
    AST_DECL_SPECS,
    AST_SPEC_QUAL_LIST,
    AST_INIT_DECLOR_LIST,
    AST_INIT_DECLOR,
    //   storage-class-specifiers
    AST_STG_TYPEDEF,
    //   type-specifiers
    AST_TYPE_VOID,
    AST_TYPE_CHAR,
    AST_TYPE_INT,
    AST_TYPE_STRUCT,
    AST_TYPE_ENUM,
    AST_TYPEDEF_NAME,
    AST_TYPE_NAME,
    //   struct-declaration
    AST_STRUCT_NAME,
    AST_STRUCT_DECL_LIST,
    AST_STRUCT_DECL,
    AST_STRUCT_DECLOR_LIST,
    //    enum-declaration
    AST_ENUM_NAME,
    AST_ENUMOR_LIST,
    AST_ENUMOR,
    AST_ENUM_CONST,
    //   declarators
    AST_IDENT_DECLOR,
    AST_ABS_DECLOR,
    AST_PTR_DECLOR,
    AST_ARRAY_DECLOR,
    AST_FUNC_DECLOR,
    AST_PARAM_LIST,
    AST_PARAM_DECL,
    //   initialization
    AST_INIT_LIST,

    // statement
    AST_CMPD_STMT,
    AST_RET_STMT,
    AST_EXPR_STMT,

    // expression
    //   assignment-expression
    AST_ASSIGN_EXPR,
    //   logical-OR-expression
    AST_LOR_EXPR,
    //   logical-AND-expression
    AST_LAND_EXPR,
    //   equality-expression
    AST_EQUAL_EXPR,
    AST_NEQUAL_EXPR,
    //   additive-expression
    AST_ADD_EXPR,
    AST_SUB_EXPR,
    //   multiplicative-expression
    AST_MUL_EXPR,
    AST_DIV_EXPR,
    AST_MOD_EXPR,
    //   casr-expression
    AST_CAST_EXPR,
    //   unary-expression
    AST_ADDR_EXPR,
    AST_INDIR_EXPR,
    AST_LNOT_EXPR,
    //   postfix-expression
    AST_SUBSC_EXPR,
    AST_CALL_EXPR,
    AST_MEMBER_EXPR,
    AST_TOMEMBER_EXPR,
    AST_ARG_LIST,
    //   primary-expression
    AST_IDENT_EXPR,
    AST_INT_EXPR,
    AST_CHAR_EXPR,
    AST_STRING_EXPR,
} AstType;

typedef struct Ast {
    AstType type;
    char* ident_name;
    IntegerLiteral* iliteral;
    StringLiteral* sliteral;
    Vector* children;
} Ast;

extern BaseType t_ast;

Ast* new_ast(AstType type, int num_children, ...);
Ast* new_identifier_ast(AstType type, char* name);
Ast* new_iliteral_ast(AstType type, IntegerLiteral* iliteral);
Ast* new_sliteral_ast(AstType type, StringLiteral* sliteral);
Ast* ast_copy(Ast* ast);
int ast_isexpr(Ast* ast);
void delete_ast(Ast* ast);

#endif
