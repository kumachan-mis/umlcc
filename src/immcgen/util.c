#include "./util.h"
#include "../immc/immc.h"

#include <stdarg.h>
#include <stdio.h>

void append_children_immcode(Immcgen* immcgen, Vector* codes) {
    Srt* srt = immcgen->srt;

    int num_children = vector_size(srt->children);
    for (int i = 0; i < num_children; i++) {
        immcgen->srt = vector_at(srt->children, i);
        Vector* sub_codes = immcgen_generate_immcode(immcgen);
        immcgen->srt = srt;

        vector_extend(codes, sub_codes);
        delete_vector(sub_codes);
    }
}

void append_child_immcode(Immcgen* immcgen, Vector* codes, int index) {
    Srt* srt = immcgen->srt;

    immcgen->srt = vector_at(srt->children, index);
    Vector* sub_codes = immcgen_generate_immcode(immcgen);
    immcgen->srt = srt;

    vector_extend(codes, sub_codes);
    delete_vector(sub_codes);
}

void update_non_void_expr_register(Immcgen* immcgen, ImmcOpe* dst) {
    immcgen->expr_reg_suffix = dst->suffix;
    if (immcgen->expr_reg != NULL) {
        delete_immcreg(immcgen->expr_reg);
    }
    immcgen->expr_reg = immcreg_copy(dst->reg);
}

void update_void_expr_register(Immcgen* immcgen) {
    immcgen->expr_reg_suffix = IMMC_SUFFIX_NONE;
    if (immcgen->expr_reg != NULL) {
        delete_immcreg(immcgen->expr_reg);
    }
    immcgen->expr_reg = NULL;
}

ImmcOpe* gen_child_int_immcope(Immcgen* immcgen, Vector* codes, int index) {
    Srt* srt = immcgen->srt;
    Srt* child_srt = vector_at(srt->children, index);

    DType* cast_dtype = NULL;
    if (child_srt->type == SRT_CAST_EXPR) {
        cast_dtype = child_srt->dtype;
    }
    while (child_srt->type == SRT_CAST_EXPR) {
        immcgen->srt = child_srt;
        index = 0;
        child_srt = vector_at(child_srt->children, index);
    }

    if (child_srt->type == SRT_INT_EXPR || child_srt->type == SRT_CHAR_EXPR) {
        immcgen->srt = srt;
        DType* iliteral_dtype = cast_dtype == NULL ? child_srt->dtype : cast_dtype;
        IntegerLiteral* iliteral = create_dtyped_iliteral(iliteral_dtype, child_srt->iliteral);
        ImmcSuffix suffix = immcsuffix_get(dtype_nbytes(iliteral_dtype));
        return new_int_immcope(suffix, iliteral);
    }

    append_child_immcode(immcgen, codes, index);
    immcgen->srt = srt;

    if (cast_dtype == NULL) {
        ImmcRegister* reg = immcreg_copy(immcgen->expr_reg);
        return new_reg_immcope(immcgen->expr_reg_suffix, reg);
    }

    ImmcRegister* src_reg = immcreg_copy(immcgen->expr_reg);
    ImmcOpe* src = new_reg_immcope(immcgen->expr_reg_suffix, src_reg);

    int cast_is_unsigned = dtype_isunsignedinteger(cast_dtype);
    ImmcSuffix cast_suffix = immcsuffix_get(dtype_nbytes(cast_dtype));

    immcgen->next_reg_id++;
    ImmcRegister* dst_reg = new_immcreg(immcgen->next_reg_id, cast_is_unsigned);
    ImmcOpe* dst = new_reg_immcope(cast_suffix, dst_reg);

    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));
    return immcope_copy(dst);
}

ImmcOpe* gen_child_reg_immcope(Immcgen* immcgen, Vector* codes, int index) {
    ImmcOpe* src = gen_child_int_immcope(immcgen, codes, index);
    if (src->type == IMMC_OPERAND_REG) {
        return src;
    }

    immcgen->next_reg_id++;
    ImmcRegister* dst_reg = new_immcreg(immcgen->next_reg_id, iliteral_isunsigned(src->iliteral));
    ImmcOpe* dst = new_reg_immcope(src->suffix, dst_reg);

    vector_push(codes, new_inst_immc(IMMC_INST_LOAD, dst, src, NULL));
    return immcope_copy(dst);
}

ImmcOpe* gen_child_ptr_immcope(Immcgen* immcgen, Vector* codes, int index) {
    Srt* child_srt = vector_at(immcgen->srt->children, index);

    int memory_or_label = 0;
    if (child_srt->type == SRT_ADDR_EXPR) {
        child_srt = vector_at(child_srt->children, 0);
        if (child_srt->type == SRT_IDENT_EXPR) {
            memory_or_label = 1;
        }
    }

    if (memory_or_label) {
        Symbol* symbol = symboltable_search(immcgen->symbol_table, child_srt->ident_name);
        if (symbol->type == SYMBOL_LABEL) {
            return new_label_immcope(new_string(symbol->name));
        }
        return new_mem_immcope(symbol->memory_offset);
    }

    append_child_immcode(immcgen, codes, index);
    return new_ptr_immcope(immcgen->next_reg_id);
}

ImmcOpe* create_dest_reg_immcope(Immcgen* immcgen) {
    ImmcSuffix suffix = immcsuffix_get(dtype_nbytes(immcgen->srt->dtype));
    int is_unsigned = dtype_isunsignedinteger(immcgen->srt->dtype);
    immcgen->next_reg_id++;
    return new_reg_immcope(suffix, new_immcreg(immcgen->next_reg_id, is_unsigned));
}

IntegerLiteral* create_dtyped_iliteral(DType* dtype, IntegerLiteral* iliteral) {
    int rank = dtype_scalar_rank(dtype);

    if (dtype_isunsignedinteger(dtype)) {
        unsigned long long value =
            iliteral_isunsigned(iliteral) ? iliteral->unsigned_value : (unsigned long long)iliteral->signed_value;
        IntegerLiteralType type = iliteral_type_get(rank, 1);
        return new_unsigned_iliteral(type, value);
    } else {
        long long value = iliteral_isunsigned(iliteral) ? (long long)iliteral->unsigned_value : iliteral->signed_value;
        IntegerLiteralType type = iliteral_type_get(rank, 0);
        return new_signed_iliteral(type, value);
    }
}