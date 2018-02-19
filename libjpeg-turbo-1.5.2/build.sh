#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  mkdir -p BUILD
  (cd BUILD && ../SRC/configure && make -j $JOBS)
}

wget -O libjpeg-turbo-1.5.2.tar.gz https://sourceforge.net/projects/libjpeg-turbo/files/1.5.2/libjpeg-turbo-1.5.2.tar.gz/download && mkdir -p SRC && tar xzf libjpeg-turbo-1.5.2.tar.gz -C SRC --strip 1 && rm -f libjpeg-turbo-1.5.2.tar.gz
build_lib
build_fuzzer
set -x
$CXX $CXXFLAGS -std=c++11 $SCRIPT_DIR/libjpeg_turbo_fuzzer.cc -I SRC BUILD/.libs/libturbojpeg.a $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE
#$CC -DTRACKPF -c -g $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I $LIBFUZZER_SRC/standalone
#$CXX $CXXFLAGS -DTRACKPF -std=c++11 $SCRIPT_DIR/libjpeg_turbo_fuzzer.cc StandaloneFuzzTargetMain.o -I $LIBFUZZER_SRC/standalone -I SRC BUILD/.libs/libturbojpeg.a -o $EXECUTABLE_NAME_BASE-pf
