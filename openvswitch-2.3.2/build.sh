#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && ./boot.sh && ./configure && make -j $JOBS)
}

get_git_tag https://github.com/openvswitch/ovs.git v2.3.2 SRC
build_lib
build_fuzzer
set -x
$CXX $CXXFLAGS $SCRIPT_DIR/target.cc -I BUILD -I BUILD/lib/ -I BUILD/include/ BUILD/lib/.libs/libopenvswitch.a $LIB_FUZZING_ENGINE -lz -o $EXECUTABLE_NAME_BASE
