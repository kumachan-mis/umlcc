#ifndef UMLCC_IMMCGEN_UTIL_H
#define UMLCC_IMMCGEN_UTIL_H

#include "../vector/vector.h"
#include "./immcgen.h"

void append_children_immcode(Immcgen* immcgen, Vector* codes);
void append_child_immcode(Immcgen* immcgen, Vector* codes, int index);
void append_child_jmp_true_immcode(Immcgen* immcgen, Vector* codes, int index, ImmcOpe* label);
void append_child_jmp_false_immcode(Immcgen* immcgen, Vector* codes, int index, ImmcOpe* label);
void update_non_void_expr_register(Immcgen* immcgen, ImmcOpe* dst);
void update_void_expr_register(Immcgen* immcgen);
ImmcOpe* gen_child_int_immcope(Immcgen* immcgen, Vector* codes, int index);
ImmcOpe* gen_child_reg_immcope(Immcgen* immcgen, Vector* codes, int index);
ImmcOpe* gen_child_ptr_immcope(Immcgen* immcgen, Vector* codes, int index);
ImmcOpe* create_dest_reg_immcope(Immcgen* immcgen);
IntegerLiteral* create_dtyped_iliteral(DType* dtype, IntegerLiteral* iliteral);

#endif
