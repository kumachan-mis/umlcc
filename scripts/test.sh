BASE_DIR=$(cd $(dirname $0)/.. && pwd)

FIXTURES_DIR=${BASE_DIR}/test-fixtures
TARGET=${BASE_DIR}/bin/umlcc
TESTLIB=${BASE_DIR}/scripts/testlib.c

INPUT=main.c
OUTPUT=main.s
BINARY=main.out
EXPECTED=expected.txt
ACTUAL=actual.txt

if [ ! -f ${TARGET} ]
then
    printf "Build umlcc before running end-to-end test\n"
    exit 1
fi

exit_code=0

for fixture_dir in ${FIXTURES_DIR}/*
do
    testcase=$(basename ${fixture_dir})
    printf "TEST: %s\n" ${testcase}

    if [ ! -f ${fixture_dir}/${INPUT} ] || [ ! -f ${fixture_dir}/${EXPECTED} ]
    then
        printf "SKIP (no test files)\n"
        printf "\n"
        continue
    fi

    input=${fixture_dir}/${INPUT}
    output=${fixture_dir}/${OUTPUT}
    binary=${fixture_dir}/${BINARY}
    expected=${fixture_dir}/${EXPECTED}
    actual=${fixture_dir}/${ACTUAL}

    rm -f ${output} ${binary} ${actual}

    ${TARGET} ${input} ${output}
    gcc -o ${binary} ${output} ${TESTLIB}
    ${binary} > ${actual}
    test_diff=$(diff -u ${expected} ${actual})

    if [ "${test_diff}" = "" ]
    then
        printf "\033[0;32mPASS\033[0m\n"
    else
        printf "\033[0;31mFAIL\033[0m\n"
        printf "%s\n" "${test_diff}"
        exit_code=1
    fi
    printf "\n"
done

exit ${exit_code}
