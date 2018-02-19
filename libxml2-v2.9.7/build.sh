#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

VERSION_STRING=2.9.7

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && CCLD="$CXX $CXXFLAGS" ./configure --without-python && make -j $JOBS)
}

#wget ftp://xmlsoft.org/libxml2/libxml2-sources-$VERSION_STRING.tar.gz &> /dev/null && mkdir -p SRC; tar xzf libxml2-sources-$VERSION_STRING.tar.gz --strip 1 -C SRC
#get_git_revision https://github.com/mcarpenter/afl be3e88d639da5350603f6c0fee06970128504342 afl
build_lib
build_fuzzer
set -x
$CXX $CXXFLAGS -std=c++11  $SCRIPT_DIR/target.cc -I BUILD/include BUILD/.libs/libxml2.a $LIB_FUZZING_ENGINE -lz -llzma -o $EXECUTABLE_NAME_BASE
$CC -DTRACKPF -c -g $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I $LIBFUZZER_SRC/standalone
$CXX $CXXFLAGS -DTRACKPF -std=c++11 $SCRIPT_DIR/target.cc StandaloneFuzzTargetMain.o -I $LIBFUZZER_SRC/standalone -I BUILD/include BUILD/.libs/libxml2.a -lz -llzma -o $EXECUTABLE_NAME_BASE-pf
