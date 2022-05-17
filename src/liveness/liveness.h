#ifndef _UMLCC_LIVENESS_H_
#define _UMLCC_LIVENESS_H_

#include "../common/common.h"

typedef struct _RegLiveness {
    int first_def_index;
    int last_use_index;
} RegLiveness;

extern BaseType t_regliveness;

RegLiveness* new_regliveness();
void delete_regliveness(RegLiveness* regliveness);
RegLiveness* regliveness_copy(RegLiveness* regliveness);
int regliveness_isinit(RegLiveness* regliveness);

#endif
