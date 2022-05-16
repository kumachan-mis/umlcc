int create_param(int x);

void func_param_function(void function(void)) {
    function();
}

void zero_param_function(void) {}

int six_param_function(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int seven_param_function(int a, int b, int c, int d, int e, int f, int g) {
    return a + b + c + d + e + f + g;
}

int eight_param_function(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

void simple_function_call() {
    func_param_function(zero_param_function);
    six_param_function(1, 2, 3, 4, 5, 6);
    seven_param_function(1, 2, 3, 4, 5, 6, 7);
    eight_param_function(1, 2, 3, 4, 5, 6, 7, 8);
}

void reg_saving_function_call() {
    int x = create_param(1) + create_param(2);
    six_param_function(1, 2, 3, 4, create_param(5), 6);
    six_param_function(1, 2, 3, create_param(4), create_param(5), 6);
    six_param_function(1, 2, 3, 4, create_param(5), create_param(6));
    seven_param_function(1, 2, 3, 4, 5, create_param(6), 7);
    eight_param_function(1, 2, 3, 4, x + 5, 6, create_param(7), 8);
}
