#include "./test_dtype.h"
#include "../../src/dtype/dtype.h"

void test_new_integer_dtype_int(void);
void test_new_integer_dtype_char(void);
void test_new_pointer_dtype(void);
void test_new_array_dtype(void);
void test_new_function_dtype(void);
void test_new_decoration_dtype(void);
void test_socket_pointer_dtype(void);
void test_socket_array_dtype(void);
void test_socket_function_dtype(void);
void test_socket_decoration_dtype(void);
void test_socket_nested_dtype(void);
void test_socket_null_dtype(void);
void test_dtype_equals_int(void);
void test_dtype_equals_char(void);
void test_dtype_equals_pointer(void);
void test_dtype_equals_array(void);
void test_dtype_equals_function(void);
void test_dtype_equals_decoration(void);
void test_dtype_equals_diff_type(void);
void test_dtype_equals_pointer_diff_to_dtype(void);
void test_dtype_equals_array_diff_of_dtype(void);
void test_dtype_equals_array_diff_size(void);
void test_dtype_equals_function_diff_num_params(void);
void test_dtype_equals_function_diff_param_dtype(void);
void test_dtype_equals_function_diff_param_name(void);
void test_dtype_equals_function_diff_param_order(void);
void test_dtype_equals_decoration_diff_deco(void);
void test_dtype_equals_decoration_diff_deco_dtype(void);
void test_dtype_isinteger(void);
void test_dtype_isarithmetic(void);
void test_dtype_isscalar(void);
void test_dtype_isaggregate(void);
void test_dtype_isobject(void);
void test_dtype_nbytes(void);

CU_Suite* add_test_suite_dtype(void) {
    CU_Suite* suite = CU_add_suite("test_suite_dtype", NULL, NULL);
    CU_ADD_TEST(suite, test_new_integer_dtype_int);
    CU_ADD_TEST(suite, test_new_integer_dtype_char);
    CU_ADD_TEST(suite, test_new_pointer_dtype);
    CU_ADD_TEST(suite, test_new_array_dtype);
    CU_ADD_TEST(suite, test_new_function_dtype);
    CU_ADD_TEST(suite, test_new_decoration_dtype);
    CU_ADD_TEST(suite, test_socket_pointer_dtype);
    CU_ADD_TEST(suite, test_socket_array_dtype);
    CU_ADD_TEST(suite, test_socket_function_dtype);
    CU_ADD_TEST(suite, test_socket_decoration_dtype);
    CU_ADD_TEST(suite, test_socket_nested_dtype);
    CU_ADD_TEST(suite, test_socket_null_dtype);
    CU_ADD_TEST(suite, test_dtype_equals_int);
    CU_ADD_TEST(suite, test_dtype_equals_char);
    CU_ADD_TEST(suite, test_dtype_equals_pointer);
    CU_ADD_TEST(suite, test_dtype_equals_array);
    CU_ADD_TEST(suite, test_dtype_equals_function);
    CU_ADD_TEST(suite, test_dtype_equals_decoration);
    CU_ADD_TEST(suite, test_dtype_equals_diff_type);
    CU_ADD_TEST(suite, test_dtype_equals_pointer_diff_to_dtype);
    CU_ADD_TEST(suite, test_dtype_equals_array_diff_of_dtype);
    CU_ADD_TEST(suite, test_dtype_equals_array_diff_size);
    CU_ADD_TEST(suite, test_dtype_equals_function_diff_num_params);
    CU_ADD_TEST(suite, test_dtype_equals_function_diff_param_dtype);
    CU_ADD_TEST(suite, test_dtype_equals_function_diff_param_name);
    CU_ADD_TEST(suite, test_dtype_equals_function_diff_param_order);
    CU_ADD_TEST(suite, test_dtype_equals_decoration_diff_deco);
    CU_ADD_TEST(suite, test_dtype_equals_decoration_diff_deco_dtype);
    CU_ADD_TEST(suite, test_dtype_isinteger);
    CU_ADD_TEST(suite, test_dtype_isarithmetic);
    CU_ADD_TEST(suite, test_dtype_isscalar);
    CU_ADD_TEST(suite, test_dtype_isaggregate);
    CU_ADD_TEST(suite, test_dtype_isobject);
    CU_ADD_TEST(suite, test_dtype_nbytes);
    return suite;
}

void test_new_integer_dtype_int(void) {
    DType* dtype = new_integer_dtype(DTYPE_INT);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            DType* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_INT);
        CU_ASSERT_PTR_NULL(dtype->pointer);
        CU_ASSERT_PTR_NULL(dtype->array);
        CU_ASSERT_PTR_NULL(dtype->function);
        CU_ASSERT_PTR_NULL(dtype->decoration);
    }

    delete_dtype(dtype);
}

void test_new_integer_dtype_char(void) {
    DType* dtype = new_integer_dtype(DTYPE_CHAR);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            DType* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_CHAR);
        CU_ASSERT_PTR_NULL(dtype->pointer);
        CU_ASSERT_PTR_NULL(dtype->array);
        CU_ASSERT_PTR_NULL(dtype->function);
        CU_ASSERT_PTR_NULL(dtype->decoration);
    }

    delete_dtype(dtype);
}

void test_new_pointer_dtype(void) {
    DType* to_dtype = new_integer_dtype(DTYPE_CHAR);
    DType* dtype = new_pointer_dtype(to_dtype);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            DType* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);
            to_dtype = copied_dtype->pointer->to_dtype;
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_POINTER);
        CU_ASSERT_PTR_EQUAL(dtype->pointer->to_dtype, to_dtype);
        CU_ASSERT_PTR_NULL(dtype->array);
        CU_ASSERT_PTR_NULL(dtype->function);
        CU_ASSERT_PTR_NULL(dtype->decoration);
    }

    delete_dtype(dtype);
}

void test_new_array_dtype(void) {
    DType* of_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = new_array_dtype(of_dtype, 5);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            DType* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);
            of_dtype = copied_dtype->array->of_dtype;
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_ARRAY);
        CU_ASSERT_PTR_NULL(dtype->pointer);
        CU_ASSERT_PTR_EQUAL(dtype->array->of_dtype, of_dtype);
        CU_ASSERT_EQUAL(dtype->array->size, 5);
        CU_ASSERT_PTR_NULL(dtype->function);
        CU_ASSERT_PTR_NULL(dtype->decoration);
    }

    delete_dtype(dtype);
}

void test_new_function_dtype(void) {
    char* param_name = new_string("param");
    DType* param_dtype = new_integer_dtype(DTYPE_CHAR);
    DParam* dparam = new_dparam(param_name, param_dtype);
    Vector* params = new_vector(&t_dparam);
    vector_push(params, dparam);
    DType* return_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = new_function_dtype(params, return_dtype);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            DType* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);

            dparam = vector_at(copied_dtype->function->params, 0);
            param_name = dparam->name;
            param_dtype = dparam->dtype;

            params = copied_dtype->function->params;
            return_dtype = copied_dtype->function->return_dtype;
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_FUNCTION);
        CU_ASSERT_PTR_NULL(dtype->pointer);
        CU_ASSERT_PTR_NULL(dtype->array);
        CU_ASSERT_PTR_EQUAL(dtype->function->params, params);
        CU_ASSERT_STRING_EQUAL(dparam->name, "param");
        CU_ASSERT_PTR_EQUAL(dparam->dtype, param_dtype);
        CU_ASSERT_PTR_EQUAL(dtype->function->return_dtype, return_dtype);
        CU_ASSERT_PTR_NULL(dtype->decoration);
    }

    delete_dtype(dtype);
}

void test_new_decoration_dtype(void) {
    DType* deco_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = new_decoration_dtype(deco_dtype);
    dtype->decoration->typedef_flag = 1;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            DType* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);
            deco_dtype = copied_dtype->decoration->deco_dtype;
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_DECORATION);
        CU_ASSERT_PTR_NULL(dtype->pointer);
        CU_ASSERT_PTR_NULL(dtype->array);
        CU_ASSERT_PTR_NULL(dtype->function);
        CU_ASSERT_PTR_EQUAL(dtype->decoration->deco_dtype, deco_dtype);
        CU_ASSERT_TRUE(dtype->decoration->typedef_flag);
    }

    delete_dtype(dtype);
}

void test_socket_pointer_dtype(void) {
    DType* to_dtype = new_integer_dtype(DTYPE_CHAR);
    DType* dtype = new_socket_pointer_dtype();

    CU_ASSERT_EQUAL(dtype->type, DTYPE_POINTER);
    CU_ASSERT_PTR_NULL(dtype->pointer->to_dtype);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    dtype = dtype_connect(dtype, to_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_POINTER);
    CU_ASSERT_PTR_EQUAL(dtype->pointer->to_dtype, to_dtype);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    delete_dtype(dtype);
}

void test_socket_array_dtype(void) {
    DType* of_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = new_socket_array_dtype(7);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_ARRAY);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype);
    CU_ASSERT_EQUAL(dtype->array->size, 7);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    dtype = dtype_connect(dtype, of_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_ARRAY);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_EQUAL(dtype->array->of_dtype, of_dtype);
    CU_ASSERT_EQUAL(dtype->array->size, 7);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    delete_dtype(dtype);
}

void test_socket_function_dtype(void) {
    char* param_name = new_string("param");
    DType* param_dtype = new_integer_dtype(DTYPE_CHAR);
    DParam* dparam = new_dparam(param_name, param_dtype);
    Vector* params = new_vector(&t_dparam);
    vector_push(params, dparam);
    DType* return_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = new_socket_function_dtype(params);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_FUNCTION);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_EQUAL(dtype->function->params, params);
    CU_ASSERT_STRING_EQUAL(dparam->name, "param");
    CU_ASSERT_PTR_EQUAL(dparam->dtype, param_dtype);
    CU_ASSERT_PTR_NULL(dtype->function->return_dtype);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    dtype = dtype_connect(dtype, return_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_FUNCTION);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_EQUAL(dtype->function->params, params);
    CU_ASSERT_STRING_EQUAL(dparam->name, "param");
    CU_ASSERT_PTR_EQUAL(dparam->dtype, param_dtype);
    CU_ASSERT_PTR_EQUAL(dtype->function->return_dtype, return_dtype);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    delete_dtype(dtype);
}

void test_socket_decoration_dtype(void) {
    DType* deco_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = new_socket_decoration_dtype();

    CU_ASSERT_EQUAL(dtype->type, DTYPE_DECORATION);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration->deco_dtype);
    CU_ASSERT_FALSE(dtype->decoration->typedef_flag);

    dtype = dtype_connect(dtype, deco_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_DECORATION);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_EQUAL(dtype->decoration->deco_dtype, deco_dtype);
    CU_ASSERT_FALSE(dtype->decoration->typedef_flag);

    delete_dtype(dtype);
}

void test_socket_nested_dtype(void) {
    DType* plug_dtype = new_integer_dtype(DTYPE_CHAR);
    DType* socket_dtype = new_array_dtype(new_socket_pointer_dtype(), 10);
    DType* dtype = dtype_connect(socket_dtype, plug_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_ARRAY);
    CU_ASSERT_EQUAL(dtype->array->size, 10);
    CU_ASSERT_EQUAL(dtype->array->of_dtype->type, DTYPE_POINTER);
    CU_ASSERT_PTR_EQUAL(dtype->array->of_dtype->pointer->to_dtype, plug_dtype);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype->array);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype->function);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype->decoration);

    delete_dtype(dtype);
}

void test_socket_null_dtype(void) {
    DType* plug_dtype = new_integer_dtype(DTYPE_INT);
    DType* dtype = dtype_connect(NULL, plug_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_INT);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    delete_dtype(dtype);
}

void test_dtype_equals_int(void) {
    DType* dtype = new_integer_dtype(DTYPE_INT);
    DType* other = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_char(void) {
    DType* dtype = new_integer_dtype(DTYPE_CHAR);
    DType* other = new_integer_dtype(DTYPE_CHAR);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_pointer(void) {
    DType* dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    DType* other = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_array(void) {
    DType* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 23);
    DType* other = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 23);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function(void) {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_INT)));
    DType* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_INT)));
    DType* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* other = new_function_dtype(other_params, return_other);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_decoration(void) {
    DType* dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    dtype->decoration->typedef_flag = 1;
    DType* other = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    other->decoration->typedef_flag = 1;

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_diff_type(void) {
    DType* dtype = new_integer_dtype(DTYPE_INT);
    DType* other = new_integer_dtype(DTYPE_CHAR);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_pointer_diff_to_dtype(void) {
    DType* dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    DType* other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_array_diff_of_dtype(void) {
    DType* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 23);
    DType* other = new_array_dtype(new_integer_dtype(DTYPE_INT), 23);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_array_diff_size(void) {
    DType* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 14);
    DType* other = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 15);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_num_params(void) {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_CHAR)));
    DType* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    DType* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* other = new_function_dtype(other_params, return_other);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_param_dtype(void) {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_INT)));
    DType* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_CHAR)));
    DType* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* other = new_function_dtype(other_params, return_other);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_param_name(void) {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    vector_push(dtype_params, new_dparam(new_string("b"), new_integer_dtype(DTYPE_CHAR)));
    DType* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(other_params, new_dparam(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    DType* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* other = new_function_dtype(other_params, return_other);

    // parameter names are not used for comparison
    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_param_order(void) {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    vector_push(dtype_params, new_dparam(new_string("b"), new_integer_dtype(DTYPE_CHAR)));
    DType* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("b"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(other_params, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    DType* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    DType* other = new_function_dtype(other_params, return_other);

    // anain, parameters are compared not by name, but by position
    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_decoration_diff_deco(void) {
    DType* dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    dtype->decoration->typedef_flag = 0;
    DType* other = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    other->decoration->typedef_flag = 1;

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_decoration_diff_deco_dtype(void) {
    DType* dtype = new_decoration_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    dtype->decoration->typedef_flag = 1;
    DType* other = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    other->decoration->typedef_flag = 1;

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_isinteger(void) {
    DType* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isinteger(char_dtype));
    delete_dtype(char_dtype);

    DType* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isinteger(int_dtype));
    delete_dtype(int_dtype);

    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(dtype_isinteger(pointer_dtype));
    delete_dtype(pointer_dtype);

    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_FALSE(dtype_isinteger(array_dtype));
    delete_dtype(array_dtype);

    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isinteger(func_dtype));
    delete_dtype(func_dtype);

    DType* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isinteger(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isarithmetic(void) {
    DType* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isarithmetic(char_dtype));
    delete_dtype(char_dtype);

    DType* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isarithmetic(int_dtype));
    delete_dtype(int_dtype);

    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(dtype_isarithmetic(pointer_dtype));
    delete_dtype(pointer_dtype);

    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_FALSE(dtype_isarithmetic(array_dtype));
    delete_dtype(array_dtype);

    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isarithmetic(func_dtype));
    delete_dtype(func_dtype);

    DType* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isarithmetic(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isscalar(void) {
    DType* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isscalar(char_dtype));
    delete_dtype(char_dtype);

    DType* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isscalar(int_dtype));
    delete_dtype(int_dtype);

    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_TRUE(dtype_isscalar(pointer_dtype));
    delete_dtype(pointer_dtype);

    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_FALSE(dtype_isscalar(array_dtype));
    delete_dtype(array_dtype);

    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isscalar(func_dtype));
    delete_dtype(func_dtype);

    DType* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isscalar(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isaggregate(void) {
    DType* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_FALSE(dtype_isaggregate(char_dtype));
    delete_dtype(char_dtype);

    DType* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_FALSE(dtype_isaggregate(int_dtype));
    delete_dtype(int_dtype);

    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(dtype_isaggregate(pointer_dtype));
    delete_dtype(pointer_dtype);

    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_TRUE(dtype_isaggregate(array_dtype));
    delete_dtype(array_dtype);

    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isaggregate(func_dtype));
    delete_dtype(func_dtype);

    DType* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isaggregate(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isobject(void) {
    DType* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isobject(char_dtype));
    delete_dtype(char_dtype);

    DType* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isobject(int_dtype));
    delete_dtype(int_dtype);

    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_TRUE(dtype_isobject(pointer_dtype));
    delete_dtype(pointer_dtype);

    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_TRUE(dtype_isobject(array_dtype));
    delete_dtype(array_dtype);

    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isobject(func_dtype));
    delete_dtype(func_dtype);

    DType* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isobject(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_nbytes(void) {
    DType* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_EQUAL(dtype_nbytes(char_dtype), 1);
    delete_dtype(char_dtype);

    DType* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_EQUAL(dtype_nbytes(int_dtype), 4);
    delete_dtype(int_dtype);

    DType* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_EQUAL(dtype_nbytes(pointer_dtype), 8);
    delete_dtype(pointer_dtype);

    DType* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 17);
    CU_ASSERT_EQUAL(dtype_nbytes(array_dtype), 68);
    delete_dtype(array_dtype);

    DType* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_EQUAL(dtype_nbytes(func_dtype), 0);
    delete_dtype(func_dtype);

    DType* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_EQUAL(dtype_nbytes(typedef_dtype), 0);
    delete_dtype(typedef_dtype);
}
