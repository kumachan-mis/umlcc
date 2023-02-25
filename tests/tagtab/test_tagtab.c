#include "./test_tagtab.h"
#include "../../src/tagtab/tagtab.h"

#include <stdlib.h>

void test_tagtable_define_enum(void);
void test_tagtable_duplicated_enum(void);
void test_tagtable_scoped_enum(void);
void test_tagtable_define_struct(void);
void test_tagtable_duplicated_struct(void);
void test_tagtable_scoped_struct(void);
void test_tagtable_copy_without_outer(void);
void test_tagtable_copy_with_outer(void);

CU_Suite* add_test_suite_tagtable(void) {
    CU_Suite* suite = CU_add_suite("add_test_suite_tagtable", NULL, NULL);
    CU_ADD_TEST(suite, test_tagtable_define_enum);
    CU_ADD_TEST(suite, test_tagtable_duplicated_enum);
    CU_ADD_TEST(suite, test_tagtable_scoped_enum);
    CU_ADD_TEST(suite, test_tagtable_define_struct);
    CU_ADD_TEST(suite, test_tagtable_duplicated_struct);
    CU_ADD_TEST(suite, test_tagtable_scoped_struct);
    CU_ADD_TEST(suite, test_tagtable_copy_without_outer);
    CU_ADD_TEST(suite, test_tagtable_copy_with_outer);
    return suite;
}

void test_tagtable_define_enum(void) {
    TagTable* table = new_tagtable();
    DType* enum_dtype = NULL;

    char* enum_name = new_string("Enum");
    Vector* enum_members = new_vector(&t_denummember);
    vector_push(enum_members, new_denummember(new_string("member"), 0));
    DType* expected_enum_dtype = new_unnamed_enum_dtype(vector_copy(enum_members));

    CU_ASSERT_TRUE(tagtable_can_define_enum(table, enum_name));

    enum_dtype = tagtable_define_enum(table, enum_name, enum_members);

    for (int i = 0; i < 2; i++) {
        if (i > 0) enum_dtype = tagtable_search_enum(table, enum_name);
        CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_enum_dtype));
    }

    delete_dtype(expected_enum_dtype);
    delete_tagtable(table);
}

void test_tagtable_duplicated_enum(void) {
    TagTable* table = new_tagtable();
    DType* enum_dtype = NULL;

    char* enum_name = new_string("Enum");
    Vector* enum_members = new_vector(&t_denummember);
    vector_push(enum_members, new_denummember(new_string("member"), 0));
    DType* expected_enum_dtype = new_unnamed_enum_dtype(vector_copy(enum_members));

    CU_ASSERT_TRUE(tagtable_can_define_enum(table, enum_name));

    enum_dtype = tagtable_define_enum(table, enum_name, enum_members);

    for (int i = 0; i < 2; i++) {
        if (i > 0) enum_dtype = tagtable_search_enum(table, enum_name);
        CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_enum_dtype));
    }

    char* dup_name = new_string("Enum");
    Vector* dup_members = new_vector(&t_denummember);
    vector_push(dup_members, new_denummember(new_string("member"), 1));

    CU_ASSERT_FALSE(tagtable_can_define_enum(table, dup_name));

    enum_dtype = tagtable_define_enum(table, dup_name, dup_members);
    CU_ASSERT_PTR_NULL(enum_dtype);

    enum_dtype = tagtable_search_enum(table, dup_name);
    CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_enum_dtype));

    free(dup_name);
    delete_vector(dup_members);
    delete_dtype(expected_enum_dtype);
    delete_tagtable(table);
}

void test_tagtable_scoped_enum(void) {
    TagTable* table = new_tagtable();
    DType* enum_dtype = NULL;

    char* outer_name = new_string("OuterEnum");
    Vector* outer_members = new_vector(&t_denummember);
    vector_push(outer_members, new_denummember(new_string("member"), 0));
    DType* expected_outer_dtype = new_unnamed_enum_dtype(vector_copy(outer_members));

    CU_ASSERT_TRUE(tagtable_can_define_enum(table, outer_name));
    tagtable_define_enum(table, outer_name, outer_members);

    char* outer_both_name = new_string("BothEnum");
    Vector* outer_both_members = new_vector(&t_denummember);
    vector_push(outer_both_members, new_denummember(new_string("member"), 0));
    DType* expected_outer_both_dtype = new_unnamed_enum_dtype(vector_copy(outer_both_members));

    CU_ASSERT_TRUE(tagtable_can_define_enum(table, outer_both_name));
    tagtable_define_enum(table, outer_both_name, outer_both_members);

    table = tagtable_enter_scope(table);

    char* inner_name = new_string("InnerEnum");
    Vector* inner_members = new_vector(&t_denummember);
    vector_push(inner_members, new_denummember(new_string("member"), 1));
    DType* expected_inner_dtype = new_unnamed_enum_dtype(vector_copy(inner_members));

    CU_ASSERT_TRUE(tagtable_can_define_enum(table, inner_name));
    tagtable_define_enum(table, inner_name, inner_members);

    char* inner_both_name = new_string("BothEnum");
    Vector* inner_both_members = new_vector(&t_denummember);
    vector_push(inner_both_members, new_denummember(new_string("member"), 1));
    DType* expected_inner_both_dtype = new_unnamed_enum_dtype(vector_copy(inner_both_members));

    CU_ASSERT_TRUE(tagtable_can_define_enum(table, inner_both_name));
    tagtable_define_enum(table, inner_both_name, inner_both_members);

    enum_dtype = tagtable_search_enum(table, outer_name);
    CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_outer_dtype));

    enum_dtype = tagtable_search_enum(table, inner_name);
    CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_inner_dtype));

    enum_dtype = tagtable_search_enum(table, inner_both_name);
    CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_inner_both_dtype));

    table = tagtable_exit_scope(table);

    enum_dtype = tagtable_search_enum(table, outer_name);
    CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_outer_dtype));

    enum_dtype = tagtable_search_enum(table, "InnerEnum");
    CU_ASSERT_PTR_NULL(enum_dtype);

    enum_dtype = tagtable_search_enum(table, outer_both_name);
    CU_ASSERT_TRUE(dtype_equals(enum_dtype, expected_outer_both_dtype));

    delete_dtype(expected_outer_dtype);
    delete_dtype(expected_outer_both_dtype);
    delete_dtype(expected_inner_dtype);
    delete_dtype(expected_inner_both_dtype);
    delete_tagtable(table);
}

void test_tagtable_define_struct(void) {
    TagTable* table = new_tagtable();
    DType* struct_dtype = NULL;

    char* struct_name = new_string("Structure");
    Vector* struct_members = new_vector(&t_dstructmember);
    vector_push(struct_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* expected_struct_dtype = new_unnamed_struct_dtype(vector_copy(struct_members));

    CU_ASSERT_TRUE(tagtable_can_define_struct(table, struct_name));

    struct_dtype = tagtable_define_struct(table, struct_name, struct_members);

    for (int i = 0; i < 2; i++) {
        if (i > 0) struct_dtype = tagtable_search_struct(table, struct_name);
        CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_struct_dtype));
    }

    delete_dtype(expected_struct_dtype);
    delete_tagtable(table);
}

void test_tagtable_duplicated_struct(void) {
    TagTable* table = new_tagtable();
    DType* struct_dtype = NULL;

    char* struct_name = new_string("Structure");
    Vector* struct_members = new_vector(&t_dstructmember);
    vector_push(struct_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* expected_struct_dtype = new_unnamed_struct_dtype(vector_copy(struct_members));

    CU_ASSERT_TRUE(tagtable_can_define_struct(table, struct_name));

    struct_dtype = tagtable_define_struct(table, struct_name, struct_members);

    for (int i = 0; i < 2; i++) {
        if (i > 0) struct_dtype = tagtable_search_struct(table, struct_name);
        CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_struct_dtype));
    }

    char* dup_name = new_string("Structure");
    Vector* dup_members = new_vector(&t_dstructmember);
    vector_push(dup_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_CHAR)));

    CU_ASSERT_FALSE(tagtable_can_define_struct(table, dup_name));

    struct_dtype = tagtable_define_struct(table, dup_name, dup_members);
    CU_ASSERT_PTR_NULL(struct_dtype);

    struct_dtype = tagtable_search_struct(table, dup_name);
    CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_struct_dtype));

    free(dup_name);
    delete_vector(dup_members);
    delete_dtype(expected_struct_dtype);
    delete_tagtable(table);
}

void test_tagtable_scoped_struct(void) {
    TagTable* table = new_tagtable();
    DType* struct_dtype = NULL;

    char* outer_name = new_string("OuterStructure");
    Vector* outer_members = new_vector(&t_dstructmember);
    vector_push(outer_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* expected_outer_dtype = new_unnamed_struct_dtype(vector_copy(outer_members));

    CU_ASSERT_TRUE(tagtable_can_define_struct(table, outer_name));
    tagtable_define_struct(table, outer_name, outer_members);

    char* outer_both_name = new_string("BothStructure");
    Vector* outer_both_members = new_vector(&t_dstructmember);
    vector_push(outer_both_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* expected_outer_both_dtype = new_unnamed_struct_dtype(vector_copy(outer_both_members));

    CU_ASSERT_TRUE(tagtable_can_define_struct(table, outer_both_name));
    tagtable_define_struct(table, outer_both_name, outer_both_members);

    table = tagtable_enter_scope(table);

    char* inner_name = new_string("InnerStructure");
    Vector* inner_members = new_vector(&t_dstructmember);
    vector_push(inner_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_CHAR)));
    DType* expected_inner_dtype = new_unnamed_struct_dtype(vector_copy(inner_members));

    CU_ASSERT_TRUE(tagtable_can_define_struct(table, inner_name));
    tagtable_define_struct(table, inner_name, inner_members);

    char* inner_both_name = new_string("BothStructure");
    Vector* inner_both_members = new_vector(&t_dstructmember);
    vector_push(inner_both_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_CHAR)));
    DType* expected_inner_both_dtype = new_unnamed_struct_dtype(vector_copy(inner_both_members));

    CU_ASSERT_TRUE(tagtable_can_define_struct(table, inner_both_name));
    tagtable_define_struct(table, inner_both_name, inner_both_members);

    struct_dtype = tagtable_search_struct(table, outer_name);
    CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_outer_dtype));

    struct_dtype = tagtable_search_struct(table, inner_name);
    CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_inner_dtype));

    struct_dtype = tagtable_search_struct(table, inner_both_name);
    CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_inner_both_dtype));

    table = tagtable_exit_scope(table);

    struct_dtype = tagtable_search_struct(table, outer_name);
    CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_outer_dtype));

    struct_dtype = tagtable_search_struct(table, "InnerStructure");
    CU_ASSERT_PTR_NULL(struct_dtype);

    struct_dtype = tagtable_search_struct(table, outer_both_name);
    CU_ASSERT_TRUE(dtype_equals(struct_dtype, expected_outer_both_dtype));

    delete_dtype(expected_outer_dtype);
    delete_dtype(expected_outer_both_dtype);
    delete_dtype(expected_inner_dtype);
    delete_dtype(expected_inner_both_dtype);
    delete_tagtable(table);
}

void test_tagtable_copy_without_outer(void) {
    TagTable* table = new_tagtable();

    char* enum_name = new_string("Enum");
    Vector* enum_members = new_vector(&t_denummember);
    vector_push(enum_members, new_denummember(new_string("member"), 0));
    DType* expected_enum_dtype = new_unnamed_enum_dtype(vector_copy(enum_members));

    tagtable_define_enum(table, enum_name, enum_members);

    char* struct_name = new_string("Structure");
    Vector* struct_members = new_vector(&t_dstructmember);
    vector_push(struct_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* expected_struct_dtype = new_unnamed_struct_dtype(vector_copy(struct_members));

    tagtable_define_struct(table, struct_name, struct_members);

    TagTable* copied_table = tagtable_copy(table);
    DType* copied_dtype = NULL;
    delete_tagtable(table);

    enum_name = new_string("Enum");
    copied_dtype = tagtable_search_enum(copied_table, enum_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_enum_dtype));
    free(enum_name);

    struct_name = new_string("Structure");
    copied_dtype = tagtable_search_struct(copied_table, struct_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_struct_dtype));
    free(struct_name);

    CU_ASSERT_PTR_NULL(copied_table->outer_scope);

    delete_dtype(expected_enum_dtype);
    delete_dtype(expected_struct_dtype);
    delete_tagtable(copied_table);
}

void test_tagtable_copy_with_outer(void) {
    TagTable* table = new_tagtable();

    char* outer_enum_name = new_string("OuterEnum");
    Vector* outer_enum_members = new_vector(&t_denummember);
    vector_push(outer_enum_members, new_denummember(new_string("member"), 0));
    DType* expected_outer_enum_dtype = new_unnamed_enum_dtype(vector_copy(outer_enum_members));

    tagtable_define_enum(table, outer_enum_name, outer_enum_members);

    char* outer_struct_name = new_string("OuterStructure");
    Vector* outer_struct_members = new_vector(&t_dstructmember);
    vector_push(outer_struct_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_INT)));
    DType* expected_outer_struct_dtype = new_unnamed_struct_dtype(vector_copy(outer_struct_members));

    tagtable_define_struct(table, outer_struct_name, outer_struct_members);

    table = tagtable_enter_scope(table);

    char* inner_enum_name = new_string("InnerEnum");
    Vector* inner_enum_members = new_vector(&t_denummember);
    vector_push(inner_enum_members, new_denummember(new_string("member"), 1));
    DType* expected_inner_enum_dtype = new_unnamed_enum_dtype(vector_copy(inner_enum_members));

    tagtable_define_enum(table, inner_enum_name, inner_enum_members);

    char* inner_struct_name = new_string("InnerStructure");
    Vector* inner_struct_members = new_vector(&t_dstructmember);
    vector_push(inner_struct_members, new_dstructmember(new_string("member"), new_integer_dtype(DTYPE_CHAR)));
    DType* expected_inner_struct_dtype = new_unnamed_struct_dtype(vector_copy(inner_struct_members));

    tagtable_define_struct(table, inner_struct_name, inner_struct_members);

    TagTable* copied_table = tagtable_copy(table);
    DType* copied_dtype = NULL;
    delete_tagtable(table);

    outer_enum_name = new_string("OuterEnum");
    copied_dtype = tagtable_search_enum(copied_table, outer_enum_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_outer_enum_dtype));
    free(outer_enum_name);

    outer_struct_name = new_string("OuterStructure");
    copied_dtype = tagtable_search_struct(copied_table, outer_struct_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_outer_struct_dtype));
    free(outer_struct_name);

    inner_enum_name = new_string("InnerEnum");
    copied_dtype = tagtable_search_enum(copied_table, inner_enum_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_inner_enum_dtype));
    free(inner_enum_name);

    inner_struct_name = new_string("InnerStructure");
    copied_dtype = tagtable_search_struct(copied_table, inner_struct_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_inner_struct_dtype));
    free(inner_struct_name);

    copied_table = tagtable_exit_scope(copied_table);

    outer_enum_name = new_string("OuterEnum");
    copied_dtype = tagtable_search_enum(copied_table, outer_enum_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_outer_enum_dtype));
    free(outer_enum_name);

    outer_struct_name = new_string("OuterStructure");
    copied_dtype = tagtable_search_struct(copied_table, outer_struct_name);
    CU_ASSERT_TRUE(dtype_equals(copied_dtype, expected_outer_struct_dtype));
    free(outer_struct_name);

    inner_enum_name = new_string("InnerEnum");
    copied_dtype = tagtable_search_enum(copied_table, inner_enum_name);
    CU_ASSERT_PTR_NULL(copied_dtype);
    free(inner_enum_name);

    inner_struct_name = new_string("InnerStructure");
    copied_dtype = tagtable_search_struct(copied_table, inner_struct_name);
    CU_ASSERT_PTR_NULL(copied_dtype);
    free(inner_struct_name);

    delete_dtype(expected_outer_enum_dtype);
    delete_dtype(expected_outer_struct_dtype);
    delete_dtype(expected_inner_enum_dtype);
    delete_dtype(expected_inner_struct_dtype);
    delete_tagtable(copied_table);
}
