#include "./suffix.h"

X64Suffix x64suffix_get(int size) {
    switch (size) {
        case 1:
            return X64_SUFFIX_BYTE;
        case 2:
            return X64_SUFFIX_WORD;
        case 4:
            return X64_SUFFIX_LONG;
        case 8:
            return X64_SUFFIX_QUAD;
        default:
            return X64_SUFFIX_NONE;
    }
}

int x64suffix_tosize(X64Suffix suffix) {
    switch (suffix) {
        case X64_SUFFIX_BYTE:
            return 1;
        case X64_SUFFIX_WORD:
            return 2;
        case X64_SUFFIX_LONG:
            return 4;
        case X64_SUFFIX_QUAD:
            return 8;
        default:
            return 0;
    }
}

char x64suffix_tochar(X64Suffix suffix) {
    switch (suffix) {
        case X64_SUFFIX_BYTE:
            return 'b';
        case X64_SUFFIX_WORD:
            return 'w';
        case X64_SUFFIX_LONG:
            return 'l';
        case X64_SUFFIX_QUAD:
            return 'q';
        default:
            return '\0';
    }
}

X64Suffix x64suffix_greater(X64Suffix a, X64Suffix b) {
    return a >= b ? a : b;
}
