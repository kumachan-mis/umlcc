#ifndef UMLCC_IMMC_LABEL_H
#define UMLCC_IMMC_LABEL_H

typedef enum ImmcLabelType {
    IMMC_LABEL_FUNCTION,
    IMMC_LABEL_VARIABLE,
    IMMC_LABEL_NORMAL,
} ImmcLabelType;

typedef enum ImmcVisibility {
    IMMC_VIS_NONE,
    IMMC_VIS_LOCAL,
    IMMC_VIS_GLOBAL,
} ImmcVisibility;

typedef struct ImmcLabel {
    ImmcLabelType type;
    ImmcVisibility visibility;
    char* name;
} ImmcLabel;

#ifdef UMLCC_IMMC_H_PRIVATE
ImmcLabel* new_immclabel(ImmcLabelType type, ImmcVisibility visibility, char* name);
ImmcLabel* immclabel_copy(ImmcLabel* immclabel);
char* immclabel_tostring(ImmcLabel* immclabel);
void delete_immclabel(ImmcLabel* immclabel);
#endif

#endif
