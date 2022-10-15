#include "./suffix.h"

ImmcSuffix immcsuffix_get(int nbytes) {
    switch (nbytes) {
        case 1:
            return IMMC_SUFFIX_BYTE;
        case 2:
            return IMMC_SUFFIX_WORD;
        case 4:
            return IMMC_SUFFIX_LONG;
        case 8:
            return IMMC_SUFFIX_QUAD;
        default:
            return IMMC_SUFFIX_NONE;
    }
}

int immcsuffix_tonbytes(ImmcSuffix suffix) {
    switch (suffix) {
        case IMMC_SUFFIX_BYTE:
            return 1;
        case IMMC_SUFFIX_WORD:
            return 2;
        case IMMC_SUFFIX_LONG:
            return 4;
        case IMMC_SUFFIX_QUAD:
            return 8;
        default:
            return 0;
    }
}

char immcsuffix_tochar(ImmcSuffix suffix) {
    switch (suffix) {
        case IMMC_SUFFIX_BYTE:
            return 'b';
        case IMMC_SUFFIX_WORD:
            return 'w';
        case IMMC_SUFFIX_LONG:
            return 'l';
        case IMMC_SUFFIX_QUAD:
            return 'q';
        default:
            return '\0';
    }
}

ImmcSuffix immcsuffix_greater(ImmcSuffix a, ImmcSuffix b) {
    return a >= b ? a : b;
}
