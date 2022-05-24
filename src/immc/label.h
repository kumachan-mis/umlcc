#ifndef UMLCC_IMMC_LABEL_H
#define UMLCC_IMMC_LABEL_H

typedef enum ImmcLabelType {
    LABEL_FUNCTION,
    LABEL_VARIABLE,
    LABEL_NORMAL,
} ImmcLabelType;
typedef enum ImmcLabelVis {
    LABELVIS_GLOBAL,
    LABELVIS_LOCAL,
    LABELVIS_DEFAULT,
} ImmcLabelVis;

typedef struct ImmcLabel {
    ImmcLabelType type;
    ImmcLabelVis visibility;
    char* name;
} ImmcLabel;

ImmcLabel* new_immclabel(ImmcLabelType type, ImmcLabelVis visibility, char* name);
ImmcLabel* immclabel_copy(ImmcLabel* immclabel);
char* immclabel_tostring(ImmcLabel* immclabel);
void delete_immclabel(ImmcLabel* immclabel);

#endif
