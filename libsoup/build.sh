#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && libtoolize --force && ./boot.sh && ./configure && make -j $JOBS)
}

build_cov() {
  rm -rf COV
  cp -rf SRC COV
  (cd COV && libtoolize --force && ./boot.sh && CFLAGS="-fprofile-instr-generate -fcoverage-mapping" ./configure && make -j)
}

build_cov_target() {
  $CC -fprofile-instr-generate -fcoverage-mapping $SCRIPT_DIR/"$1" $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I COV -I COV/lib -I COV/include COV/lib/.libs/libopenvswitch.a -lz -lcrypto -lssl -lpthread -lrt -lm -o coverage-$(echo "$1" | sed 's/\.c//g')
}

get_git_tag git://git.gnome.org/libsoup 2.60.0 SRC
build_lib
build_fuzzer
set -x
$CC $CFLAGS -c -g $SCRIPT_DIR/target-parse-req.c -I BUILD/libsoup
#$CXX $CXXFLAGS target.o BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE
