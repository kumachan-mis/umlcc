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

ResolverReturnDType* new_resolverret_dtype(DType* dtype) {
    ResolverReturnDType* resolverret_dtype = malloc(sizeof(ResolverReturnDType));
    resolverret_dtype->dtype = dtype;
    resolverret_dtype->errs = NULL;
    return resolverret_dtype;
}

ResolverReturnDType* new_resolverret_dtype_errors(Vector* errs) {
    ResolverReturnDType* resolverret_dtype = malloc(sizeof(ResolverReturnDType));
    resolverret_dtype->dtype = NULL;
    resolverret_dtype->errs = errs;
    return resolverret_dtype;
}

void resolverret_dtype_assign(DType** dtype_ptr, Vector** errs_ptr, ResolverReturnDType* resolverret_dtype) {
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

ResolverReturnDMembers* new_resolverret_dmembers(Vector* dmembers) {
    ResolverReturnDMembers* resolverret_dmembers = malloc(sizeof(ResolverReturnDMembers));
    resolverret_dmembers->dmembers = dmembers;
    resolverret_dmembers->errs = NULL;
    return resolverret_dmembers;
}

ResolverReturnDMembers* new_resolverret_dmembers_errors(Vector* errs) {
    ResolverReturnDMembers* resolverret_dmembers = malloc(sizeof(ResolverReturnDMembers));
    resolverret_dmembers->dmembers = NULL;
    resolverret_dmembers->errs = errs;
    return resolverret_dmembers;
}

void resolverret_dmembers_assign(Vector** dmembers_ptr, Vector** errs_ptr,
                                 ResolverReturnDMembers* resolverret_dmembers) {
    *dmembers_ptr = resolverret_dmembers->dmembers;
    *errs_ptr = resolverret_dmembers->errs;
    free(resolverret_dmembers);
}

ResolverReturnDMember* new_resolverret_dmember(DMember* dmember) {
    ResolverReturnDMember* resolverret_dmember = malloc(sizeof(ResolverReturnDMember));
    resolverret_dmember->dmember = dmember;
    resolverret_dmember->errs = NULL;
    return resolverret_dmember;
}

ResolverReturnDMember* new_resolverret_dmember_errors(Vector* errs) {
    ResolverReturnDMember* resolverret_dmember = malloc(sizeof(ResolverReturnDMember));
    resolverret_dmember->dmember = NULL;
    resolverret_dmember->errs = errs;
    return resolverret_dmember;
}

void resolverret_dmember_assign(DMember** dmember_ptr, Vector** errs_ptr, ResolverReturnDMember* resolverret_dmember) {
    *dmember_ptr = resolverret_dmember->dmember;
    *errs_ptr = resolverret_dmember->errs;
    free(resolverret_dmember);
}
