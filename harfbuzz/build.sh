#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

git clone --depth 1 https://anongit.freedesktop.org/git/harfbuzz.git BUILD
build_fuzzer
set -x
cd BUILD
# Build the library.
libtoolize --force
$HOME/local/bin/autoreconf -fiv
./configure
make clean
make -j$(nproc) V=1 all
make -C src V=1 fuzzing

OUT=..
# Build the fuzzer.
$CXX $CXXFLAGS -std=c++11 -Isrc \
	    ./test/fuzzing/hb-fuzzer.cc -o $OUT/hb-fuzzer \
	        ../$LIB_FUZZING_ENGINE ./src/.libs/libharfbuzz-fuzzing.a

# Archive and copy to $OUT seed corpus if the build succeeded.
cd ..
cp -r ./BUILD/test/shaping/data/in-house/fonts seed_corpus
