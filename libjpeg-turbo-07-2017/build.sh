#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && autoreconf -fiv && ./configure && make -j $JOBS)
}

get_git_tag https://github.com/libjpeg-turbo/libjpeg-turbo.git 1.5.2 SRC
build_lib
build_fuzzer
set -x
$CXX $CXXFLAGS -std=c++11 $SCRIPT_DIR/libjpeg_turbo_fuzzer.cc -I BUILD BUILD/.libs/libturbojpeg.a $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE
