#ifndef UMLCC_DTYPE_DECORATION_H
#define UMLCC_DTYPE_DECORATION_H

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;
#endif

typedef struct DDecoration DDecoration;

struct DDecoration {
    int typedef_flag;
    DType* deco_dtype;
};

#ifdef UMLCC_DTYPE_H_PRIVATE
DDecoration* new_ddecoration(DType* deco_dtype);
DDecoration* new_socket_ddecoration(void);
DDecoration* ddecoration_copy(DDecoration* ddecoration);
DType* ddecoration_next(DDecoration* ddecoration);
DDecoration* ddecoration_connect(DDecoration* socket, DType* plug);
int ddecoration_equals(DDecoration* ddecoration, DDecoration* other);
void delete_ddecoration(DDecoration* ddecoration);
#endif

#endif
