#!/bin/bash

BASE_DIR=$(cd $(dirname $0)/.. && pwd)

OBJ_DIR=${BASE_DIR}/build/src/object
COV_DIR=${BASE_DIR}/cov

cd ${BASE_DIR}
for obj_file in ${OBJ_DIR}/**/*.o
do
    cov_dir=$(dirname ${obj_file} | sed "s|^${OBJ_DIR}|${COV_DIR}|")
    cov_file=$(basename ${obj_file} | sed "s|.o$|.c.gcov|")

    mkdir -p ${cov_dir}
    gcov -r ${obj_file}
    mv ${cov_file} ${cov_dir}
done
