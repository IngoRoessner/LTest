#!/bin/bash

set -e
rm -rf obj
rm -rf bin/
CUR_OUTPUT_FILE=current-output.txt
make
echo RUNNING NOW LTEST-TESTS
bin/tests/run-ltest-tests 
echo RUNNING NOW LTEST-EXAMPLE
bin/Debug/LTestExample
for f in $(find ./bin -name *.xml); do ./exec-validator-on-testxml.sh $f resources/xunit.xsd; done
