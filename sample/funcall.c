void caller(void callee(void)) {
    callee();
}

void callee(void) {
}

int main(void) {
    caller(callee);
}
