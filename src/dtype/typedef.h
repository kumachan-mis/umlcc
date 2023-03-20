#ifndef UMLCC_DTYPE_TYPEDEF_H
#define UMLCC_DTYPE_TYPEDEF_H

#ifndef UMLCC_DTYPE_H
typedef struct DType DType;

DType* dtype_copy(DType* dtype);
int dtype_equals(DType* dtype, DType* other);
void delete_dtype(DType* dtype);
#endif

typedef struct DTypedef DTypedef;

struct DTypedef {
    DType* defined_dtype;
};

#ifdef UMLCC_DTYPE_H_PRIVATE
DTypedef* new_dtypedef(DType* deco_dtype);
DTypedef* new_socket_dtypedef(void);
DTypedef* dtypedef_copy(DTypedef* dtypedef);
DType* dtypedef_next(DTypedef* dtypedef);
DTypedef* dtypedef_connect(DTypedef* socket, DType* plug);
int dtypedef_equals(DTypedef* dtypedef, DTypedef* other);
void delete_dtypedef(DTypedef* dtypedef);
#endif

#endif
