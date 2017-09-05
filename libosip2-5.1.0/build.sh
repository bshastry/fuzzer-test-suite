#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && ./configure && make -j $JOBS)
}

get_git_tag https://git.savannah.gnu.org/git/osip.git 5.0.0 SRC
build_lib
build_fuzzer
set -x
$CC $CFLAGS -c -g $SCRIPT_DIR/target-sipp.c -I include
$CXX $CXXFLAGS target-sipp.o BUILD/src/osipparser2/libosipparser2.a $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE
