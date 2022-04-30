void func_param_function(void function(void)) {
    function();
}

void zero_param_function(void) {
}

int six_param_function(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int seven_param_function(int a, int b, int c, int d, int e, int f, int g) {
    return a + b + c + d + e + f + g;
}

int eight_param_function(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

int main(void) {
    func_param_function(zero_param_function);
    six_param_function(1, 2, 3, 4, 5, 6);
    seven_param_function(1, 2, 3, 4, 5, 6, 7);
    eight_param_function(1, 2, 3, 4, 5, 6, 7, 8);
}
