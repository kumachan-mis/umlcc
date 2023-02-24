#include "./resolverret.h"

#include <stdlib.h>

ResolverReturn* new_resolverret(Srt* srt) {
    ResolverReturn* resolverret = malloc(sizeof(ResolverReturn));
    resolverret->srt = srt;
    resolverret->errs = NULL;
    return resolverret;
}

ResolverReturn* new_resolverret_errors(Vector* errs) {
    ResolverReturn* resolverret = malloc(sizeof(ResolverReturn));
    resolverret->srt = NULL;
    resolverret->errs = errs;
    return resolverret;
}

void resolverret_assign(Srt** srt_ptr, Vector** errs_ptr, ResolverReturn* resolverret) {
    *srt_ptr = resolverret->srt;
    *errs_ptr = resolverret->errs;
    free(resolverret);
}

ResolverDTypeReturn* new_resolverret_dtype(DType* dtype) {
    ResolverDTypeReturn* resolverret_dtype = malloc(sizeof(ResolverDTypeReturn));
    resolverret_dtype->dtype = dtype;
    resolverret_dtype->errs = NULL;
    return resolverret_dtype;
}

ResolverDTypeReturn* new_resolverret_dtype_errors(Vector* errs) {
    ResolverDTypeReturn* resolverret_dtype = malloc(sizeof(ResolverDTypeReturn));
    resolverret_dtype->dtype = NULL;
    resolverret_dtype->errs = errs;
    return resolverret_dtype;
}

void resolverret_dtype_assign(DType** dtype_ptr, Vector** errs_ptr, ResolverDTypeReturn* resolverret_dtype) {
    *dtype_ptr = resolverret_dtype->dtype;
    *errs_ptr = resolverret_dtype->errs;
    free(resolverret_dtype);
}

ResolverReturnDParams* new_resolverret_dparams(Vector* dparams) {
    ResolverReturnDParams* resolverret_dparams = malloc(sizeof(ResolverReturnDParams));
    resolverret_dparams->dparams = dparams;
    resolverret_dparams->errs = NULL;
    return resolverret_dparams;
}

ResolverReturnDParams* new_resolverret_dparams_errors(Vector* errs) {
    ResolverReturnDParams* resolverret_dparams = malloc(sizeof(ResolverReturnDParams));
    resolverret_dparams->dparams = NULL;
    resolverret_dparams->errs = errs;
    return resolverret_dparams;
}

void resolverret_dparams_assign(Vector** dparams_ptr, Vector** errs_ptr, ResolverReturnDParams* resolverret_dparams) {
    *dparams_ptr = resolverret_dparams->dparams;
    *errs_ptr = resolverret_dparams->errs;
    free(resolverret_dparams);
}

ResolverReturnDParam* new_resolverret_dparam(DParam* dparam) {
    ResolverReturnDParam* resolverret_dparam = malloc(sizeof(ResolverReturnDParam));
    resolverret_dparam->dparam = dparam;
    resolverret_dparam->errs = NULL;
    return resolverret_dparam;
}

ResolverReturnDParam* new_resolverret_dparam_errors(Vector* errs) {
    ResolverReturnDParam* resolverret_dparam = malloc(sizeof(ResolverReturnDParam));
    resolverret_dparam->dparam = NULL;
    resolverret_dparam->errs = errs;
    return resolverret_dparam;
}

void resolverret_dparam_assign(DParam** dparam_ptr, Vector** errs_ptr, ResolverReturnDParam* resolverret_dparam) {
    *dparam_ptr = resolverret_dparam->dparam;
    *errs_ptr = resolverret_dparam->errs;
    free(resolverret_dparam);
}

ResolverReturnDMembers* new_resolverret_dstructmembers(Vector* dstructmembers) {
    ResolverReturnDMembers* resolverret_dstructmembers = malloc(sizeof(ResolverReturnDMembers));
    resolverret_dstructmembers->dstructmembers = dstructmembers;
    resolverret_dstructmembers->errs = NULL;
    return resolverret_dstructmembers;
}

ResolverReturnDMembers* new_resolverret_dstructmembers_errors(Vector* errs) {
    ResolverReturnDMembers* resolverret_dstructmembers = malloc(sizeof(ResolverReturnDMembers));
    resolverret_dstructmembers->dstructmembers = NULL;
    resolverret_dstructmembers->errs = errs;
    return resolverret_dstructmembers;
}

void resolverret_dstructmembers_assign(Vector** dstructmembers_ptr, Vector** errs_ptr,
                                       ResolverReturnDMembers* resolverret_dstructmembers) {
    *dstructmembers_ptr = resolverret_dstructmembers->dstructmembers;
    *errs_ptr = resolverret_dstructmembers->errs;
    free(resolverret_dstructmembers);
}

ResolverReturnDMember* new_resolverret_dstructmember(DStructMember* dstructmember) {
    ResolverReturnDMember* resolverret_dstructmember = malloc(sizeof(ResolverReturnDMember));
    resolverret_dstructmember->dstructmember = dstructmember;
    resolverret_dstructmember->errs = NULL;
    return resolverret_dstructmember;
}

ResolverReturnDMember* new_resolverret_dstructmember_errors(Vector* errs) {
    ResolverReturnDMember* resolverret_dstructmember = malloc(sizeof(ResolverReturnDMember));
    resolverret_dstructmember->dstructmember = NULL;
    resolverret_dstructmember->errs = errs;
    return resolverret_dstructmember;
}

void resolverret_dstructmember_assign(DStructMember** dstructmember_ptr, Vector** errs_ptr,
                                      ResolverReturnDMember* resolverret_dstructmember) {
    *dstructmember_ptr = resolverret_dstructmember->dstructmember;
    *errs_ptr = resolverret_dstructmember->errs;
    free(resolverret_dstructmember);
}
