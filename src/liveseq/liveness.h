#ifndef _UMLCC_LIVENESS_H_
#define _UMLCC_LIVENESS_H_

#include "../common/common.h"

typedef struct _Liveness {
    int first_def_index;
    int last_use_index;
} Liveness;

extern BaseType t_liveness;

Liveness* new_liveness(int immc_index);
void delete_liveness(Liveness* liveness);
Liveness* liveness_copy(Liveness* liveness);
int liveness_isinit(Liveness* liveness);

#endif
