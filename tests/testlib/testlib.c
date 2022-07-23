#include "./testlib.h"
#include "../../src/ctoken/ctoken.h"
#include "../../src/literal/iliteral.h"
#include "../../src/literal/sliteral.h"

#include <stdlib.h>
#include <string.h>

int testlib_ctoken_equals(CToken* actual, CToken* expected);
int testlib_iliteral_equals(IntegerLiteral* actual, IntegerLiteral* expected);
int testlib_sliteral_equals(StringLiteral* actual, StringLiteral* expected);
int testlib_string_equals(char* actual, char* expected);

int testlib_ctokens_equals(Vector* actual, Vector* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    if (vector_size(actual) != vector_size(expected)) return 0;

    int num_tokens = vector_size(expected);
    for (int i = 0; i < num_tokens; i++) {
        CToken* actual_ctoken = vector_at(actual, i);
        CToken* expected_ctoken = vector_at(expected, i);
        if (!testlib_ctoken_equals(actual_ctoken, expected_ctoken)) return 0;
    }
    return 1;
}

int testlib_ctoken_equals(CToken* actual, CToken* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type &&
            testlib_string_equals(actual->ident_name, expected->ident_name) &&
            testlib_iliteral_equals(actual->iliteral, expected->iliteral) &&
            testlib_sliteral_equals(actual->sliteral, expected->sliteral));
}

int testlib_iliteral_equals(IntegerLiteral* actual, IntegerLiteral* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;

    return (actual->type == expected->type && actual->is_unsigned == expected->is_unsigned &&
            actual->signed_value == expected->signed_value &&
            actual->unsigned_value == expected->unsigned_value);
}

int testlib_sliteral_equals(StringLiteral* actual, StringLiteral* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    if (actual->size != expected->size) return 0;
    return memcmp(actual->value, expected->value, expected->size) == 0;
}

int testlib_string_equals(char* actual, char* expected) {
    if (actual == NULL || expected == NULL) return actual == NULL && expected == NULL;
    return strcmp(actual, expected) == 0;
}
