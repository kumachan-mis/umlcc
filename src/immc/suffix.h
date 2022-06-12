#ifndef UMLCC_IMMC_SUFFIX_H
#define UMLCC_IMMC_SUFFIX_H

typedef enum ImmcSuffix {
    IMMC_SUFFIX_NONE,
    IMMC_SUFFIX_BYTE,
    IMMC_SUFFIX_WORD,
    IMMC_SUFFIX_LONG,
    IMMC_SUFFIX_QUAD
} ImmcSuffix;

ImmcSuffix immcsuffix_get(int size);
int immcsuffix_tosize(ImmcSuffix suffix);
char immcsuffix_tochar(ImmcSuffix suffix);
ImmcSuffix immcsuffix_greater(ImmcSuffix a, ImmcSuffix b);

#endif
