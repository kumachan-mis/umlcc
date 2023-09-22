void put_int(int x);
void put_str(char* x);

void put_fizzbuzz(int n) {
    if (n == 0) {
        return;
    }

    put_fizzbuzz(n - 1);

    if (n % 15 == 0) {
        put_str("FizzBuzz");
    } else if (n % 3 == 0) {
        put_str("Fizz");
    } else if (n % 5 == 0) {
        put_str("Buzz");
    } else {
        put_int(n);
    }
}

int main(void) {
    put_fizzbuzz(15);
    return 0;
}
