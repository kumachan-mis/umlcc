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
    AST_TYPE_LONG,
    AST_TYPE_UNSIGNED,
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
    AST_CASE_STMT,
    AST_DEFAULT_STMT,
    AST_CONTINUE_STMT,
    AST_BREAK_STMT,
    AST_RET_STMT,
    AST_EXPR_STMT,
    AST_NULL_STMT,
    AST_IF_STMT,
    AST_SWITCH_STMT,
    AST_WHILE_STMT,
    AST_FOR_STMT,

    // expression
    //   assignment-expression
    AST_ASSIGN_EXPR,
    AST_MUL_ASSIGN_EXPR,
    AST_DIV_ASSIGN_EXPR,
    AST_MOD_ASSIGN_EXPR,
    AST_ADD_ASSIGN_EXPR,
    AST_SUB_ASSIGN_EXPR,
    AST_LSHIFT_ASSIGN_EXPR,
    AST_RSHIFT_ASSIGN_EXPR,
    AST_AND_ASSIGN_EXPR,
    AST_XOR_ASSIGN_EXPR,
    AST_OR_ASSIGN_EXPR,
    //   conditional-expression
    AST_COND_EXPR,
    //   logical-OR-expression
    AST_LOR_EXPR,
    //   logical-AND-expression
    AST_LAND_EXPR,
    //   inclusive-OR-expression
    AST_OR_EXPR,
    //   exclusive-OR-expression
    AST_XOR_EXPR,
    //   AND-expression
    AST_AND_EXPR,
    //   equality-expression
    AST_EQUAL_EXPR,
    AST_NEQUAL_EXPR,
    //  relational-expression
    AST_LESS_EXPR,
    AST_GREATER_EXPR,
    AST_LESSEQ_EXPR,
    AST_GREATEREQ_EXPR,
    //   shift-expression
    AST_LSHIFT_EXPR,
    AST_RSHIFT_EXPR,
    //   additive-expression
    AST_ADD_EXPR,
    AST_SUB_EXPR,
    //   multiplicative-expression
    AST_MUL_EXPR,
    AST_DIV_EXPR,
    AST_MOD_EXPR,
    //   cast-expression
    AST_CAST_EXPR,
    //   unary-expression
    AST_PREINC_EXPR,
    AST_PREDEC_EXPR,
    AST_ADDR_EXPR,
    AST_INDIR_EXPR,
    AST_POS_EXPR,
    AST_NEG_EXPR,
    AST_NOT_EXPR,
    AST_LNOT_EXPR,
    AST_SIZEOF_EXPR,
    //   postfix-expression
    AST_SUBSC_EXPR,
    AST_CALL_EXPR,
    AST_MEMBER_EXPR,
    AST_TOMEMBER_EXPR,
    AST_POSTINC_EXPR,
    AST_POSTDEC_EXPR,
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
