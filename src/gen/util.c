#include "../vector/vector.h"

#include <stdarg.h>

void append_code(Vector* codes, char* format, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, format);

    char* code = malloc((50 + 1) * sizeof(char));
    vsprintf(code, format, arg_ptr);
    code = realloc(code, (strlen(code) + 1) * sizeof(char));
    vector_push(codes, code);

    va_end(arg_ptr);
}