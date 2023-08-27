#include "./util.h"

int min_multiple_of(int n, int base) {
    return (n + base - 1) / base * base;
}
