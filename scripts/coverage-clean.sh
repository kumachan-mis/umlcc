#!/bin/bash

BASE_DIR=$(cd $(dirname $0)/.. && pwd)

COV_DIR=${BASE_DIR}/cov

rm -rf ${COV_DIR}
