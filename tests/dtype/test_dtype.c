#include "./test_dtype.h"
#include "../../src/dtype/dtype.h"

void test_new_integer_dtype_int();
void test_new_integer_dtype_char();
void test_new_pointer_dtype();
void test_new_array_dtype();
void test_new_function_dtype();
void test_new_decoration_dtype();
void test_socket_pointer_dtype();
void test_socket_array_dtype();
void test_socket_function_dtype();
void test_socket_decoration_dtype();
void test_socket_nested_dtype();
void test_socket_null_dtype();
void test_dtype_equals_int();
void test_dtype_equals_char();
void test_dtype_equals_pointer();
void test_dtype_equals_array();
void test_dtype_equals_function();
void test_dtype_equals_decoration();
void test_dtype_equals_diff_type();
void test_dtype_equals_pointer_diff_to_dtype();
void test_dtype_equals_array_diff_of_dtype();
void test_dtype_equals_array_diff_size();
void test_dtype_equals_function_diff_num_params();
void test_dtype_equals_function_diff_param_dtype();
void test_dtype_equals_function_diff_param_name();
void test_dtype_equals_function_diff_param_order();
void test_dtype_equals_decoration_diff_deco();
void test_dtype_equals_decoration_diff_deco_dtype();
void test_dtype_isinteger();
void test_dtype_isarithmetic();
void test_dtype_isscalar();
void test_dtype_isaggregate();
void test_dtype_isobject();
void test_dtype_size();

CU_Suite* add_test_suite_dtype() {
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
    CU_ADD_TEST(suite, test_dtype_size);
    return suite;
}

void test_new_integer_dtype_int() {
    Dtype* dtype = new_integer_dtype(DTYPE_INT);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Dtype* copied_dtype = dtype_copy(dtype);
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

void test_new_integer_dtype_char() {
    Dtype* dtype = new_integer_dtype(DTYPE_CHAR);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Dtype* copied_dtype = dtype_copy(dtype);
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

void test_new_pointer_dtype() {
    Dtype* to_dtype = new_integer_dtype(DTYPE_CHAR);
    Dtype* dtype = new_pointer_dtype(to_dtype);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Dtype* copied_dtype = dtype_copy(dtype);
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

void test_new_array_dtype() {
    Dtype* of_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = new_array_dtype(of_dtype, 5);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Dtype* copied_dtype = dtype_copy(dtype);
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

void test_new_function_dtype() {
    char* param_name = new_string("param");
    Dtype* param_dtype = new_integer_dtype(DTYPE_CHAR);
    DParam* dparam = new_dparam(param_name, param_dtype);
    Vector* params = new_vector(&t_dparam);
    vector_push(params, dparam);
    Dtype* return_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = new_function_dtype(params, return_dtype);

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Dtype* copied_dtype = dtype_copy(dtype);
            delete_dtype(dtype);

            dparam = vector_at(copied_dtype->function->params, 0);
            param_name = dparam->ident_name;
            param_dtype = dparam->dtype;

            params = copied_dtype->function->params;
            return_dtype = copied_dtype->function->return_dtype;
            dtype = copied_dtype;
        }
        CU_ASSERT_EQUAL(dtype->type, DTYPE_FUNCUCTION);
        CU_ASSERT_PTR_NULL(dtype->pointer);
        CU_ASSERT_PTR_NULL(dtype->array);
        CU_ASSERT_PTR_EQUAL(dtype->function->params, params);
        CU_ASSERT_STRING_EQUAL(dparam->ident_name, "param");
        CU_ASSERT_PTR_EQUAL(dparam->dtype, param_dtype);
        CU_ASSERT_PTR_EQUAL(dtype->function->return_dtype, return_dtype);
        CU_ASSERT_PTR_NULL(dtype->decoration);
    }

    delete_dtype(dtype);
}

void test_new_decoration_dtype() {
    Dtype* deco_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = new_decoration_dtype(deco_dtype);
    dtype->decoration->typedef_flag = 1;

    for (int i = 0; i < 2; i++) {
        if (i > 0) {
            Dtype* copied_dtype = dtype_copy(dtype);
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

void test_socket_pointer_dtype() {
    Dtype* to_dtype = new_integer_dtype(DTYPE_CHAR);
    Dtype* dtype = new_socket_pointer_dtype();

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

void test_socket_array_dtype() {
    Dtype* of_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = new_socket_array_dtype(7);

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

void test_socket_function_dtype() {
    char* param_name = new_string("param");
    Dtype* param_dtype = new_integer_dtype(DTYPE_CHAR);
    DParam* dparam = new_dparam(param_name, param_dtype);
    Vector* params = new_vector(&t_dparam);
    vector_push(params, dparam);
    Dtype* return_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = new_socket_function_dtype(params);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_FUNCUCTION);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_EQUAL(dtype->function->params, params);
    CU_ASSERT_STRING_EQUAL(dparam->ident_name, "param");
    CU_ASSERT_PTR_EQUAL(dparam->dtype, param_dtype);
    CU_ASSERT_PTR_NULL(dtype->function->return_dtype);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    dtype = dtype_connect(dtype, return_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_FUNCUCTION);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_EQUAL(dtype->function->params, params);
    CU_ASSERT_STRING_EQUAL(dparam->ident_name, "param");
    CU_ASSERT_PTR_EQUAL(dparam->dtype, param_dtype);
    CU_ASSERT_PTR_EQUAL(dtype->function->return_dtype, return_dtype);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    delete_dtype(dtype);
}

void test_socket_decoration_dtype() {
    Dtype* deco_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = new_socket_decoration_dtype();

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

void test_socket_nested_dtype() {
    Dtype* plug_dtype = new_integer_dtype(DTYPE_CHAR);
    Dtype* socket_dtype = new_array_dtype(new_socket_pointer_dtype(), 10);
    Dtype* dtype = dtype_connect(socket_dtype, plug_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_ARRAY);
    CU_ASSERT_EQUAL(dtype->array->size, 10);
    CU_ASSERT_EQUAL(dtype->array->of_dtype->type, DTYPE_POINTER);
    CU_ASSERT_PTR_EQUAL(dtype->array->of_dtype->pointer->to_dtype, plug_dtype);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype->array);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype->function);
    CU_ASSERT_PTR_NULL(dtype->array->of_dtype->decoration);

    delete_dtype(dtype);
}

void test_socket_null_dtype() {
    Dtype* plug_dtype = new_integer_dtype(DTYPE_INT);
    Dtype* dtype = dtype_connect(NULL, plug_dtype);

    CU_ASSERT_EQUAL(dtype->type, DTYPE_INT);
    CU_ASSERT_PTR_NULL(dtype->pointer);
    CU_ASSERT_PTR_NULL(dtype->array);
    CU_ASSERT_PTR_NULL(dtype->function);
    CU_ASSERT_PTR_NULL(dtype->decoration);

    delete_dtype(dtype);
}

void test_dtype_equals_int() {
    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    Dtype* other = new_integer_dtype(DTYPE_INT);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_char() {
    Dtype* dtype = new_integer_dtype(DTYPE_CHAR);
    Dtype* other = new_integer_dtype(DTYPE_CHAR);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_pointer() {
    Dtype* dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    Dtype* other = new_pointer_dtype(new_integer_dtype(DTYPE_INT));

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_array() {
    Dtype* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 23);
    Dtype* other = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 23);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function() {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_INT)));
    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_INT)));
    Dtype* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* other = new_function_dtype(other_params, return_other);

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_decoration() {
    Dtype* dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    dtype->decoration->typedef_flag = 1;
    Dtype* other = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    other->decoration->typedef_flag = 1;

    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_diff_type() {
    Dtype* dtype = new_integer_dtype(DTYPE_INT);
    Dtype* other = new_integer_dtype(DTYPE_CHAR);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_pointer_diff_to_dtype() {
    Dtype* dtype = new_pointer_dtype(new_integer_dtype(DTYPE_INT));
    Dtype* other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_array_diff_of_dtype() {
    Dtype* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 23);
    Dtype* other = new_array_dtype(new_integer_dtype(DTYPE_INT), 23);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_array_diff_size() {
    Dtype* dtype = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 14);
    Dtype* other = new_array_dtype(new_integer_dtype(DTYPE_CHAR), 15);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_num_params() {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_CHAR)));
    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    Dtype* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* other = new_function_dtype(other_params, return_other);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_param_dtype() {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_INT)));
    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("param"), new_integer_dtype(DTYPE_CHAR)));
    Dtype* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* other = new_function_dtype(other_params, return_other);

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_param_name() {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    vector_push(dtype_params, new_dparam(new_string("b"), new_integer_dtype(DTYPE_CHAR)));
    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("x"), new_integer_dtype(DTYPE_INT)));
    vector_push(other_params, new_dparam(new_string("y"), new_integer_dtype(DTYPE_CHAR)));
    Dtype* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* other = new_function_dtype(other_params, return_other);

    // parameter names are not used for comparison
    CU_ASSERT_TRUE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_function_diff_param_order() {
    Vector* dtype_params = new_vector(&t_dparam);
    vector_push(dtype_params, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    vector_push(dtype_params, new_dparam(new_string("b"), new_integer_dtype(DTYPE_CHAR)));
    Dtype* return_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* dtype = new_function_dtype(dtype_params, return_dtype);

    Vector* other_params = new_vector(&t_dparam);
    vector_push(other_params, new_dparam(new_string("b"), new_integer_dtype(DTYPE_CHAR)));
    vector_push(other_params, new_dparam(new_string("a"), new_integer_dtype(DTYPE_INT)));
    Dtype* return_other = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    Dtype* other = new_function_dtype(other_params, return_other);

    // anain, parameters are compared not by name, but by position
    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_decoration_diff_deco() {
    Dtype* dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    dtype->decoration->typedef_flag = 0;
    Dtype* other = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    other->decoration->typedef_flag = 1;

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_equals_decoration_diff_deco_dtype() {
    Dtype* dtype = new_decoration_dtype(new_pointer_dtype(new_integer_dtype(DTYPE_INT)));
    dtype->decoration->typedef_flag = 1;
    Dtype* other = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    other->decoration->typedef_flag = 1;

    CU_ASSERT_FALSE(dtype_equals(dtype, other));

    delete_dtype(dtype);
    delete_dtype(other);
}

void test_dtype_isinteger() {
    Dtype* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isinteger(char_dtype));
    delete_dtype(char_dtype);

    Dtype* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isinteger(int_dtype));
    delete_dtype(int_dtype);

    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(dtype_isinteger(pointer_dtype));
    delete_dtype(pointer_dtype);

    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_FALSE(dtype_isinteger(array_dtype));
    delete_dtype(array_dtype);

    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isinteger(func_dtype));
    delete_dtype(func_dtype);

    Dtype* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isinteger(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isarithmetic() {
    Dtype* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isarithmetic(char_dtype));
    delete_dtype(char_dtype);

    Dtype* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isarithmetic(int_dtype));
    delete_dtype(int_dtype);

    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(dtype_isarithmetic(pointer_dtype));
    delete_dtype(pointer_dtype);

    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_FALSE(dtype_isarithmetic(array_dtype));
    delete_dtype(array_dtype);

    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isarithmetic(func_dtype));
    delete_dtype(func_dtype);

    Dtype* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isarithmetic(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isscalar() {
    Dtype* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isscalar(char_dtype));
    delete_dtype(char_dtype);

    Dtype* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isscalar(int_dtype));
    delete_dtype(int_dtype);

    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_TRUE(dtype_isscalar(pointer_dtype));
    delete_dtype(pointer_dtype);

    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_FALSE(dtype_isscalar(array_dtype));
    delete_dtype(array_dtype);

    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isscalar(func_dtype));
    delete_dtype(func_dtype);

    Dtype* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isscalar(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isaggregate() {
    Dtype* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_FALSE(dtype_isaggregate(char_dtype));
    delete_dtype(char_dtype);

    Dtype* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_FALSE(dtype_isaggregate(int_dtype));
    delete_dtype(int_dtype);

    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_FALSE(dtype_isaggregate(pointer_dtype));
    delete_dtype(pointer_dtype);

    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_TRUE(dtype_isaggregate(array_dtype));
    delete_dtype(array_dtype);

    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isaggregate(func_dtype));
    delete_dtype(func_dtype);

    Dtype* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isaggregate(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_isobject() {
    Dtype* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_TRUE(dtype_isobject(char_dtype));
    delete_dtype(char_dtype);

    Dtype* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_TRUE(dtype_isobject(int_dtype));
    delete_dtype(int_dtype);

    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_TRUE(dtype_isobject(pointer_dtype));
    delete_dtype(pointer_dtype);

    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 3);
    CU_ASSERT_TRUE(dtype_isobject(array_dtype));
    delete_dtype(array_dtype);

    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_FALSE(dtype_isobject(func_dtype));
    delete_dtype(func_dtype);

    Dtype* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_FALSE(dtype_isobject(typedef_dtype));
    delete_dtype(typedef_dtype);
}

void test_dtype_size() {
    Dtype* char_dtype = new_integer_dtype(DTYPE_CHAR);
    CU_ASSERT_EQUAL(dtype_size(char_dtype), 1);
    delete_dtype(char_dtype);

    Dtype* int_dtype = new_integer_dtype(DTYPE_INT);
    CU_ASSERT_EQUAL(dtype_size(int_dtype), 4);
    delete_dtype(int_dtype);

    Dtype* pointer_dtype = new_pointer_dtype(new_integer_dtype(DTYPE_CHAR));
    CU_ASSERT_EQUAL(dtype_size(pointer_dtype), 8);
    delete_dtype(pointer_dtype);

    Dtype* array_dtype = new_array_dtype(new_integer_dtype(DTYPE_INT), 17);
    CU_ASSERT_EQUAL(dtype_size(array_dtype), 68);
    delete_dtype(array_dtype);

    Dtype* func_dtype = new_function_dtype(new_vector(&t_dparam), new_integer_dtype(DTYPE_INT));
    CU_ASSERT_EQUAL(dtype_size(func_dtype), 0);
    delete_dtype(func_dtype);

    Dtype* typedef_dtype = new_decoration_dtype(new_integer_dtype(DTYPE_INT));
    typedef_dtype->decoration->typedef_flag = 1;
    CU_ASSERT_EQUAL(dtype_size(typedef_dtype), 0);
    delete_dtype(typedef_dtype);
}
