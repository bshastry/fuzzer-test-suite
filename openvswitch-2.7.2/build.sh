#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && libtoolize --force && ./boot.sh && ./configure && make -j $JOBS)
}

get_git_tag https://github.com/openvswitch/ovs.git v2.7.2 SRC
build_lib
build_fuzzer
set -x
$CC $CFLAGS -c -g $SCRIPT_DIR/target.c -I BUILD -I BUILD/lib -I BUILD/include
$CXX $CXXFLAGS target.o BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE
$CC $CFLAGS -c -g $SCRIPT_DIR/target-ofpl.c -I BUILD -I BUILD/lib -I BUILD/include
$CXX $CXXFLAGS target-ofpl.o BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE-ofp
$CC $CFLAGS -c -g $SCRIPT_DIR/target-ct.c -I BUILD -I BUILD/lib -I BUILD/include
$CXX $CXXFLAGS target-ct.o BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE-ct
$CC $CFLAGS -c -g $SCRIPT_DIR/target-jsonp.c -I BUILD -I BUILD/lib -I BUILD/include
$CXX $CXXFLAGS target-jsonp.o BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE-jsonp
