#!/bin/bash

set -e
rm -rf LTestUse/bin
rm -rf LTestUse/obj
rm -rf obj
rm -rf bin/
CUR_OUTPUT_FILE=current-output.txt
make > $CUR_OUTPUT_FILE && cd LTestUse && make >> "../$CUR_OUTPUT_FILE" && bin/Debug/LTestUse >> "../$CUR_OUTPUT_FILE"
cd ..
./exec-validator-on-testxml.sh LTestUse/bin/TEST-ExternTestSuiteExample.xml resources/xunit.xsd
#diff ../$CUR_OUTPUT_FILE ../expected-output.txt
