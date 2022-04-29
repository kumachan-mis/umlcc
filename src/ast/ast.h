#ifndef _UMLCC_AST_H_
#define _UMLCC_AST_H_


#include "../vector/vector.h"

typedef enum _AstType {
    // translation unit
    AST_TRAS_UNIT,

    // function definition
    AST_FUNC_DEF,

    // declaration
    AST_DECL,
    AST_DECL_SPECIFIERS,
    AST_INIT_DECLOR_LIST,
    AST_INIT_DECLOR,
    //   type-specifiers 
    AST_TYPE_INT,
    //   declarators
    AST_IDENT_DECLOR,
    AST_FUNC_DECLOR,
    AST_PARAM_LIST,
    AST_PARAM_DECL,

    // statement
    AST_CMPD_STMT,
    AST_EXPR_STMT,

    // expression
    //   assignment-expression
    AST_ASSIGN_EXPR,
    //   additive-expression
    AST_ADD_EXPR,
    AST_SUB_EXPR,
    //   multiplicative-expression
    AST_MUL_EXPR,
    AST_DIV_EXPR,
    AST_MOD_EXPR,
    //   postfix-expression
    AST_CALL_EXPR,
    AST_ARG_LIST,
    //   primary-expression
    AST_IDENT_EXPR,
    AST_INT_EXPR,
} AstType;

typedef struct _Ast {
    AstType type;
    char* ident_name;
    int value_int;
    Vector* children;
} Ast;

Ast* new_ast(AstType type, int num_children, ...);
Ast* new_identifier_ast(AstType type, char* name);
Ast* new_integer_ast(AstType type, int value);
void delete_ast(Ast* ast);

#endif
