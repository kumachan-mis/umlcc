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

typedef struct ResolverReturnDMembers {
    Vector* dmembers;
    Vector* errs;
} ResolverReturnDMembers;

typedef struct ResolverReturnDMember {
    DMember* dmember;
    Vector* errs;
} ResolverReturnDMember;

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
ResolverReturnDMembers* new_resolverret_dmembers(Vector* dmembers);
ResolverReturnDMembers* new_resolverret_dmembers_errors(Vector* errs);
void resolverret_dmembers_assign(Vector** dmembers_ptr, Vector** errs_ptr,
                                 ResolverReturnDMembers* resolverret_dmembers);
ResolverReturnDMember* new_resolverret_dmember(DMember* dmember);
ResolverReturnDMember* new_resolverret_dmember_errors(Vector* errs);
void resolverret_dmember_assign(DMember** dmembers_ptr, Vector** errs_ptr, ResolverReturnDMember* resolverret_dmember);

#endif
