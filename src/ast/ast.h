#ifndef UMLCC_AST_H
#define UMLCC_AST_H

#include "../common/type.h"
#include "../literal/sliteral.h"
#include "../vector/vector.h"

typedef enum AstType {
    // translation unit
    AST_TRAS_UNIT,

    // function definition
    AST_FUNC_DEF,

    // declaration
    AST_DECL,
    AST_DECL_SPECIFIERS,
    AST_INIT_DECLOR_LIST,
    AST_INIT_DECLOR,
    //   storage-class-specifiers
    AST_STG_TYPEDEF,
    //   type-specifiers
    AST_TYPE_CHAR,
    AST_TYPE_INT,
    AST_TYPEDEF_NAME,
    //   declarators
    AST_IDENT_DECLOR,
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
    //   unary-expression
    AST_ADDR_EXPR,
    AST_INDIR_EXPR,
    AST_LNOT_EXPR,
    //   postfix-expression
    AST_SUBSC_EXPR,
    AST_CALL_EXPR,
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
    int value_int;
    StringLiteral* sliteral;
    Vector* children;
} Ast;

extern BaseType t_ast;

Ast* new_ast(AstType type, int num_children, ...);
Ast* new_identifier_ast(AstType type, char* name);
Ast* new_integer_ast(AstType type, int value);
Ast* new_sliteral_ast(AstType type, StringLiteral* sliteral);
Ast* ast_copy(Ast* ast);
void delete_ast(Ast* ast);

#endif
