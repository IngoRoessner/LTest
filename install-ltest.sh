#!/bin/bash
#
# Copies all ltest-files to unix std folders for static libraries and
# headers. Needs probably root access rights. For adustments see below.

LIB_FOLDER=/usr/local/lib
INCLUDE_FOLDER=/usr/local/include


REAL_INCLUDE_FOLDER=$INCLUDE_FOLDER/ltest


cp libltest.a $LIB_FOLDER
mkdir $REAL_INCLUDE_FOLDER
for f in $(find src -name "*.h");
do
        name="${f#*/}"
        TARGET=$REAL_INCLUDE_FOLDER/${name}
        mkdir --parents $(dirname $TARGET)
        cp $f $TARGET
done
