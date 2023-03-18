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

ResolverReturnDStructMembers* new_resolverret_dstructmembers(Vector* dstructmembers) {
    ResolverReturnDStructMembers* resolverret_dstructmembers = malloc(sizeof(ResolverReturnDStructMembers));
    resolverret_dstructmembers->dstructmembers = dstructmembers;
    resolverret_dstructmembers->errs = NULL;
    return resolverret_dstructmembers;
}

ResolverReturnDStructMembers* new_resolverret_dstructmembers_errors(Vector* errs) {
    ResolverReturnDStructMembers* resolverret_dstructmembers = malloc(sizeof(ResolverReturnDStructMembers));
    resolverret_dstructmembers->dstructmembers = NULL;
    resolverret_dstructmembers->errs = errs;
    return resolverret_dstructmembers;
}

void resolverret_dstructmembers_assign(Vector** dstructmembers_ptr, Vector** errs_ptr,
                                       ResolverReturnDStructMembers* resolverret_dstructmembers) {
    *dstructmembers_ptr = resolverret_dstructmembers->dstructmembers;
    *errs_ptr = resolverret_dstructmembers->errs;
    free(resolverret_dstructmembers);
}

ResolverReturnDStructMember* new_resolverret_dstructmember(DStructMember* dstructmember) {
    ResolverReturnDStructMember* resolverret_dstructmember = malloc(sizeof(ResolverReturnDStructMember));
    resolverret_dstructmember->dstructmember = dstructmember;
    resolverret_dstructmember->errs = NULL;
    return resolverret_dstructmember;
}

ResolverReturnDStructMember* new_resolverret_dstructmember_errors(Vector* errs) {
    ResolverReturnDStructMember* resolverret_dstructmember = malloc(sizeof(ResolverReturnDStructMember));
    resolverret_dstructmember->dstructmember = NULL;
    resolverret_dstructmember->errs = errs;
    return resolverret_dstructmember;
}

void resolverret_dstructmember_assign(DStructMember** dstructmember_ptr, Vector** errs_ptr,
                                      ResolverReturnDStructMember* resolverret_dstructmember) {
    *dstructmember_ptr = resolverret_dstructmember->dstructmember;
    *errs_ptr = resolverret_dstructmember->errs;
    free(resolverret_dstructmember);
}

ResolverReturnDEnumMembers* new_resolverret_denummembers(Vector* denummembers) {
    ResolverReturnDEnumMembers* resolverret_denummembers = malloc(sizeof(ResolverReturnDEnumMembers));
    resolverret_denummembers->denummembers = denummembers;
    resolverret_denummembers->errs = NULL;
    return resolverret_denummembers;
}

ResolverReturnDEnumMembers* new_resolverret_denummembers_errors(Vector* errs) {
    ResolverReturnDEnumMembers* resolverret_denummembers = malloc(sizeof(ResolverReturnDEnumMembers));
    resolverret_denummembers->denummembers = NULL;
    resolverret_denummembers->errs = errs;
    return resolverret_denummembers;
}

void resolverret_denummembers_assign(Vector** denummembers_ptr, Vector** errs_ptr,
                                     ResolverReturnDEnumMembers* resolverret_denummembers) {
    *denummembers_ptr = resolverret_denummembers->denummembers;
    *errs_ptr = resolverret_denummembers->errs;
    free(resolverret_denummembers);
}

ResolverReturnDEnumMember* new_resolverret_denummember(DEnumMember* denummember) {
    ResolverReturnDEnumMember* resolverret_denummember = malloc(sizeof(ResolverReturnDEnumMember));
    resolverret_denummember->denummember = denummember;
    resolverret_denummember->errs = NULL;
    return resolverret_denummember;
}

ResolverReturnDEnumMember* new_resolverret_denummember_errors(Vector* errs) {
    ResolverReturnDEnumMember* resolverret_denummember = malloc(sizeof(ResolverReturnDEnumMember));
    resolverret_denummember->denummember = NULL;
    resolverret_denummember->errs = errs;
    return resolverret_denummember;
}

void resolverret_denummember_assign(DEnumMember** denummember_ptr, Vector** errs_ptr,
                                    ResolverReturnDEnumMember* resolverret_denummember) {
    *denummember_ptr = resolverret_denummember->denummember;
    *errs_ptr = resolverret_denummember->errs;
    free(resolverret_denummember);
}
