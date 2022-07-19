#!/bin/bash

BASE_DIR=$(cd $(dirname $0)/.. && pwd)

COBJ_DIR=${BASE_DIR}/build/src/cobject
COV_DIR=${BASE_DIR}/cov

cd ${BASE_DIR}
for cobj_file in ${COBJ_DIR}/**/*.o
do
    cov_dir=$(dirname ${cobj_file} | sed "s|^${COBJ_DIR}|${COV_DIR}|")
    cov_file=$(basename ${cobj_file} | sed "s|.o$|.c.gcov|")

    mkdir -p ${cov_dir}
    gcov -r ${cobj_file}
    mv ${cov_file} ${cov_dir}
done
