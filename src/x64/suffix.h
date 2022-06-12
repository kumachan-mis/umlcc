#ifndef UMLCC_X64_SUFFIX_H
#define UMLCC_X64_SUFFIX_H

typedef enum X64Suffix {
    X64_SUFFIX_NONE,
    X64_SUFFIX_BYTE,
    X64_SUFFIX_WORD,
    X64_SUFFIX_LONG,
    X64_SUFFIX_QUAD
} X64Suffix;

X64Suffix x64suffix_get(int size);
int x64suffix_tosize(X64Suffix suffix);
char x64suffix_tochar(X64Suffix suffix);

#endif
