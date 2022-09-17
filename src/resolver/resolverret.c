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

ResolverReturnDtype* new_resolverret_dtype(Dtype* dtype) {
    ResolverReturnDtype* resolverret_dtype = malloc(sizeof(ResolverReturnDtype));
    resolverret_dtype->dtype = dtype;
    resolverret_dtype->errs = NULL;
    return resolverret_dtype;
}

ResolverReturnDtype* new_resolverret_dtype_errors(Vector* errs) {
    ResolverReturnDtype* resolverret_dtype = malloc(sizeof(ResolverReturnDtype));
    resolverret_dtype->dtype = NULL;
    resolverret_dtype->errs = errs;
    return resolverret_dtype;
}

void resolverret_dtype_assign(Dtype** dtype_ptr, Vector** errs_ptr,
                              ResolverReturnDtype* resolverret_dtype) {
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

void resolverret_dparams_assign(Vector** dparams_ptr, Vector** errs_ptr,
                                ResolverReturnDParams* resolverret_dparams) {
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

void resolverret_dparam_assign(DParam** dparam_ptr, Vector** errs_ptr,
                               ResolverReturnDParam* resolverret_dparam) {
    *dparam_ptr = resolverret_dparam->dparam;
    *errs_ptr = resolverret_dparam->errs;
    free(resolverret_dparam);
}