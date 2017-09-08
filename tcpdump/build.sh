#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd libpcap && ./configure && make -j $JOBS)
  (cd BUILD && ./configure && make -j $JOBS)
}

git clone https://github.com/the-tcpdump-group/tcpdump.git SRC
git clone https://github.com/the-tcpdump-group/libpcap.git
build_lib
build_fuzzer
set -x
$CC $CFLAGS -c -g $SCRIPT_DIR/target.c -I BUILD -I libpcap
$CXX $CXXFLAGS -L libpcap target.o BUILD/libnetdissect.a -lssl -lcrypto -lpcap $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE
