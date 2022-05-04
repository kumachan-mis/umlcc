#ifndef _UMLCC_IMMC_LABEL_H_
#define _UMLCC_IMMC_LABEL_H_

typedef enum _ImmcLabelType {
    LABEL_FUNCTION_HEAD,
    LABEL_FUNCTION_TAIL,
    LABEL_NORMAL
} ImmcLabelType;
typedef enum _ImmcLabelVisibility {
    VISIBILITY_GLOBAL,
    VISIBILITY_LOCAL,
    VISIBILITY_DEFAULT
} ImmcLabelVisibility;

typedef struct _ImmcLabel {
    ImmcLabelType type;
    ImmcLabelVisibility visibility;
    char* label_name;
} ImmcLabel;

ImmcLabel* new_immclabel(ImmcLabelType type, ImmcLabelVisibility visibility, char* label_name);
char* immclabel_tostring(ImmcLabel* immclabel);
void delete_immclabel(ImmcLabel* immclabel);

#endif
