#ifndef UMLCC_TYPE_DECORATION_H
#define UMLCC_TYPE_DECORATION_H

typedef struct DDecoration {
    int typedef_flag;
    struct Dtype* deco_dtype;
} DDecoration;

DDecoration* new_ddecoration(struct Dtype* deco_dtype);
DDecoration* new_socket_ddecoration(void);
DDecoration* ddecoration_copy(DDecoration* ddecoration);
struct Dtype* ddecoration_next(DDecoration* ddecoration);
DDecoration* ddecoration_connect(DDecoration* socket, struct Dtype* plug);
int ddecoration_equals(DDecoration* ddecoration, DDecoration* other);
void delete_ddecoration(DDecoration* ddecoration);

#endif
