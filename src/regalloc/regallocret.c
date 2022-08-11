#include "./regallocret.h"

#include <stdlib.h>

RegAllocReturn* new_regallocret(Vector* immcs, Vector* liveseqs) {
    RegAllocReturn* regallocret = malloc(sizeof(RegAllocReturn));
    regallocret->immcs = immcs;
    regallocret->liveseqs = liveseqs;
    return regallocret;
}

void regallocret_close(RegAllocReturn* regallocret) {
    free(regallocret);
}
