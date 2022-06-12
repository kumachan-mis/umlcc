#ifndef UMLCC_X64_LABEL_H
#define UMLCC_X64_LABEL_H

typedef enum X64LabelType {
    X64_LABEL_FUNCTION,
    X64_LABEL_VARIABLE,
    X64_LABEL_NORMAL,
} X64LabelType;

typedef enum X64Visibility {
    X64_VIS_NONE,
    X64_VIS_LOCAL,
    X64_VIS_GLOBAL,
} X64Visibility;

typedef struct X64Label {
    X64LabelType type;
    X64Visibility visibility;
    char* name;
} X64Label;

X64Label* new_x64label(X64LabelType type, X64Visibility visibility, char* name);
X64Label* x64label_copy(X64Label* x64label);
char* x64label_tostring(X64Label* x64label);
void delete_x64label(X64Label* x64label);

#endif
