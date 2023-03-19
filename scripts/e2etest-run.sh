#!/bin/bash -e

BASE_DIR=$(cd $(dirname $0)/.. && pwd)

FIXTURES_DIR=${BASE_DIR}/test-fixtures
TARGET=${BASE_DIR}/bin/umlcc
TESTLIB=${FIXTURES_DIR}/_/testlib.c

INPUT=main.c
IMMEDIATE=main.i
OUTPUT=main.s
BINARY=main.out
EXPECTED=expected.txt
ACTUAL=actual.txt

RED="\x1b[1;31m"
GREEN="\x1b[1;32m"
END="\x1b[0m"

if [ ! -f ${TARGET} ]; then
    printf "Build umlcc before running end-to-end test\n"
    exit 1
fi

exit_code=0

for fixture_dir in ${FIXTURES_DIR}/*; do
    if [ ${fixture_dir} = ${FIXTURES_DIR}/_ ]; then
        continue
    fi

    testcase=$(basename ${fixture_dir})
    printf "Test: ${testcase} ..."

    if [ ! -f ${fixture_dir}/${INPUT} ] || [ ! -f ${fixture_dir}/${EXPECTED} ]; then
        echo "skipped"
        continue
    fi

    input=${fixture_dir}/${INPUT}
    immediate=${fixture_dir}/${IMMEDIATE}
    output=${fixture_dir}/${OUTPUT}
    binary=${fixture_dir}/${BINARY}
    expected=${fixture_dir}/${EXPECTED}
    actual=${fixture_dir}/${ACTUAL}

    rm -f ${immediate} ${output} ${binary} ${actual}

    ${TARGET} ${input}
    if [ $? != 0 ]; then
        continue
    fi

    gcc -o ${binary} ${output} ${TESTLIB}
    ${binary} > ${actual}

    test_diff=$(diff -u --color ${expected} ${actual})
    if [ "${test_diff}" = "" ]; then
        echo -e "${GREEN}passed${END}"
    else
        echo -e "${RED}FAILED${END}"
        echo "${test_diff}"
        exit_code=$(expr ${exit_code} + 1)
    fi
done

exit ${exit_code}
