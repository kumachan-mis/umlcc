#ifndef UMLCC_RESOLVER_RESOLVERRET_H
#define UMLCC_RESOLVER_RESOLVERRET_H

#include "../error/error.h"
#include "../srt/srt.h"

typedef struct ResolverReturn {
    Srt* srt;
    Vector* errs;
} ResolverReturn;

typedef struct ResolverDTypeReturn {
    DType* dtype;
    Vector* errs;
} ResolverDTypeReturn;

typedef struct ResolverReturnDParams {
    Vector* dparams;
    Vector* errs;
} ResolverReturnDParams;

typedef struct ResolverReturnDParam {
    DParam* dparam;
    Vector* errs;
} ResolverReturnDParam;

typedef struct ResolverReturnDStructMembers {
    Vector* dstructmembers;
    Vector* errs;
} ResolverReturnDStructMembers;

typedef struct ResolverReturnDStructMember {
    DStructMember* dstructmember;
    Vector* errs;
} ResolverReturnDStructMember;

typedef struct ResolverReturnDEnumMembers {
    Vector* denummembers;
    Vector* errs;
} ResolverReturnDEnumMembers;

typedef struct ResolverReturnDEnumMember {
    DEnumMember* denummember;
    Vector* errs;
} ResolverReturnDEnumMember;

ResolverReturn* new_resolverret(Srt* srt);
ResolverReturn* new_resolverret_errors(Vector* errs);
void resolverret_assign(Srt** srt_ptr, Vector** errs_ptr, ResolverReturn* resolverret);

ResolverDTypeReturn* new_resolverret_dtype(DType* dtype);
ResolverDTypeReturn* new_resolverret_dtype_errors(Vector* errs);
void resolverret_dtype_assign(DType** dtype_ptr, Vector** errs_ptr, ResolverDTypeReturn* resolverret_dtype);

ResolverReturnDParams* new_resolverret_dparams(Vector* dparams);
ResolverReturnDParams* new_resolverret_dparams_errors(Vector* errs);
void resolverret_dparams_assign(Vector** dparams_ptr, Vector** errs_ptr, ResolverReturnDParams* resolverret_dparams);

ResolverReturnDParam* new_resolverret_dparam(DParam* dparam);
ResolverReturnDParam* new_resolverret_dparam_errors(Vector* errs);
void resolverret_dparam_assign(DParam** dparam_ptr, Vector** errs_ptr, ResolverReturnDParam* resolverret_dparam);

ResolverReturnDStructMembers* new_resolverret_dstructmembers(Vector* dstructmembers);
ResolverReturnDStructMembers* new_resolverret_dstructmembers_errors(Vector* errs);
void resolverret_dstructmembers_assign(Vector** dstructmember_ptr, Vector** errs_ptr,
                                       ResolverReturnDStructMembers* resolverret_dstructmembers);

ResolverReturnDStructMember* new_resolverret_dstructmember(DStructMember* dstructmember);
ResolverReturnDStructMember* new_resolverret_dstructmember_errors(Vector* errs);
void resolverret_dstructmember_assign(DStructMember** dstructmember_ptr, Vector** errs_ptr,
                                      ResolverReturnDStructMember* resolverret_dstructmember);

ResolverReturnDEnumMembers* new_resolverret_denummembers(Vector* denummembers);
ResolverReturnDEnumMembers* new_resolverret_denummembers_errors(Vector* errs);
void resolverret_denummembers_assign(Vector** denummembers_ptr, Vector** errs_ptr,
                                     ResolverReturnDEnumMembers* resolverret_denummembers);

ResolverReturnDEnumMember* new_resolverret_denummember(DEnumMember* denummember);
ResolverReturnDEnumMember* new_resolverret_denummember_errors(Vector* errs);
void resolverret_denummember_assign(DEnumMember** denummember_ptr, Vector** errs_ptr,
                                    ResolverReturnDEnumMember* resolverret_denummember);

#endif
