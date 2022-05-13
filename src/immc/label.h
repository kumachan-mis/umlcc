#ifndef _UMLCC_IMMC_LABEL_H_
#define _UMLCC_IMMC_LABEL_H_

typedef enum _ImmcLabelType {
    LABEL_FUNCTION,
    LABEL_VARIABLE,
    LABEL_NORMAL,
} ImmcLabelType;
typedef enum _ImmcLabelVis {
    LABELVIS_GLOBAL,
    LABELVIS_LOCAL,
    LABELVIS_DEFAULT,
} ImmcLabelVis;

typedef struct _ImmcLabel {
    ImmcLabelType type;
    ImmcLabelVis visibility;
    char* name;
} ImmcLabel;

ImmcLabel* new_immclabel(ImmcLabelType type, ImmcLabelVis visibility, char* name);
char* immclabel_tostring(ImmcLabel* immclabel);
void delete_immclabel(ImmcLabel* immclabel);

#endif
