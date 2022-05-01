BASE_DIR=$(cd $(dirname $0)/.. && pwd)

FIXTURES_DIR=${BASE_DIR}/test-fixtures

OUTPUT=main.s
BINARY=main.out
ACTUAL=actual.txt

for fixture_dir in ${FIXTURES_DIR}/*
do
    output=${fixture_dir}/${OUTPUT}
    binary=${fixture_dir}/${BINARY}
    actual=${fixture_dir}/${ACTUAL}

    rm -f ${output} ${binary} ${actual}
done
