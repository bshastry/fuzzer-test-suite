#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

git clone https://git.libssh.org/projects/libssh.git SRC
SRC=`pwd`/SRC
build_fuzzer
set -x
mkdir -p BUILD
cd BUILD
cmake -DCMAKE_C_COMPILER="$CC" -DCMAKE_CXX_COMPILER="$CXX" \
	    -DCMAKE_C_FLAGS="$CFLAGS" -DCMAKE_CXX_FLAGS="$CXXFLAGS" \
	        -DWITH_STATIC_LIB=ON ../SRC
make "-j$(nproc)"
OUT=..

$CXX $CXXFLAGS -std=c++11 -Iinclude/ \
	    "$SRC/tests/fuzz/ssh_server_fuzzer.cpp" \
	        -o "$OUT/libssh_server_fuzzer" \
		    ../$LIB_FUZZING_ENGINE ./src/libssh.a -Wl,-Bstatic -lcrypto -lz -Wl,-Bdynamic
