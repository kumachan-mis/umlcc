#!/bin/bash

BASE_DIR=$(cd $(dirname $0)/.. && pwd)

COBJ_DIR=${BASE_DIR}/build/src/cobject
COV_DIR=${BASE_DIR}/cov
CONFIG_FILE=${BASE_DIR}/.coverage

CONFIG=$(cat ${CONFIG_FILE})
GCOV_ARGS=$(echo ${CONFIG} | jq -r ".gcovArgs[]")
RED_THRESHOLD=$(echo ${CONFIG} | jq ".colorThreshold.red")
YELLOW_THRESHOLD=$(echo ${CONFIG} | jq ".colorThreshold.yellow")
GREEN_THRESHOLD=$(echo ${CONFIG} | jq ".colorThreshold.green")

RED="\e[1;31m"
GREEN="\e[1;32m"
YELLOW="\e[1;33m"
BLUE="\e[1;34m"
END="\e[0m"

print_gcov () {
    gcov_summary=$1

    while read -r line; do
        if ! [[ ${line} =~ ^(.*):([0-9]{1,3})\.([0-9]{2})%(.*)$ ]]; then
            echo "${line}"
            continue
        fi

        left=${BASH_REMATCH[1]}
        integer_part=${BASH_REMATCH[2]}; fractional_part=${BASH_REMATCH[3]}
        right=${BASH_REMATCH[4]}

        if [ ${integer_part} -lt ${RED_THRESHOLD} ]; then
            color=${RED}
        elif [ ${integer_part} -lt ${YELLOW_THRESHOLD} ]; then
            color=${YELLOW}
        elif [ ${integer_part} -lt ${GREEN_THRESHOLD} ]; then
            color=${GREEN}
        else
            color=${BLUE}
        fi

        echo -e "${left}:${color}${integer_part}.${fractional_part}%${END}${right}"
    done <<< "${gcov_summary}"

    echo
}

cd ${BASE_DIR}
for cobj_file in ${COBJ_DIR}/**/*.o
do
    cov_dir=$(dirname ${cobj_file} | sed "s|^${COBJ_DIR}|${COV_DIR}|")
    cov_file=$(basename ${cobj_file} | sed "s|.o$|.c.gcov|")

    mkdir -p ${cov_dir}
    print_gcov "$(gcov ${GCOV_ARGS} ${cobj_file})"
    mv ${cov_file} ${cov_dir}
done
