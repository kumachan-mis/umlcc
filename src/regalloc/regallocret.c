#include "./regallocret.h"

#include <stdlib.h>

RegAllocReturn* new_regallocret(Vector* immcs, Vector* liveseqs) {
    RegAllocReturn* regallocret = malloc(sizeof(RegAllocReturn));
    regallocret->immcs = immcs;
    regallocret->liveseqs = liveseqs;
    return regallocret;
}

void regallocret_assign(Vector** immcs_ptr, Vector** liveseqs_ptr, RegAllocReturn* regallocret) {
    *immcs_ptr = regallocret->immcs;
    *liveseqs_ptr = regallocret->liveseqs;
    free(regallocret);
}
