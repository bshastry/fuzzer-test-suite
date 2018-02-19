#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

git clone --depth 1 https://github.com/aawc/unrar.git BUILD
build_fuzzer
set -x
UNRAR_SRC_DIR="."
OUT=..
cd BUILD
# build 'lib'. This builds libunrar.a and libunrar.so
# -fPIC is required for successful compilation.
make CXX=$CXX CXXFLAGS="$CXXFLAGS -fPIC" -C $UNRAR_SRC_DIR lib

# remove the .so file so that the linker links unrar statically.
rm -v $UNRAR_SRC_DIR/libunrar.so

# build fuzzer
$CXX $CXXFLAGS -std=c++11 -I. $SCRIPT_DIR/unrar_fuzzer.cc -o $OUT/unrar_fuzzer \
	     -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -DRAR_SMP -DRARDLL \
	          ../$LIB_FUZZING_ENGINE -L$UNRAR_SRC_DIR -lunrar
