#!/ bin / bash

BASE_DIR=$(cd $(dirname $0)/.. && pwd)

FIXTURES_DIR=${BASE_DIR}/test-fixtures

IMMEDIATE=main.i
OUTPUT=main.s
BINARY=main.out
ACTUAL=actual.txt

for fixture_dir in ${
    FIXTURES_DIR
}
/*
do
    immediate=${fixture_dir}/${IMMEDIATE}
    output=${fixture_dir}/${OUTPUT}
    binary=${fixture_dir}/${BINARY}
    actual=${fixture_dir}/${ACTUAL}

    rm -f ${immediate} ${output} ${binary} ${actual}
done
