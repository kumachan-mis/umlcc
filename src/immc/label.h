#ifndef _UMLCC_IMMC_LABEL_H_
#define _UMLCC_IMMC_LABEL_H_

typedef enum _ImmcLabelType { LABEL_GLOBAL, LABEL_LOCAL } ImmcLabelType;

typedef struct _ImmcLabel {
    ImmcLabelType type;
    char* label_name;
} ImmcLabel;

ImmcLabel* new_immclabel(ImmcLabelType type, char* label_name);
char* immclabel_tostring(ImmcLabel* immclabel);
void delete_immclabel(ImmcLabel* immclabel);

#endif
