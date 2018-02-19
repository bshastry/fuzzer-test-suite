#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh
OUT=..

#git clone --depth 1 https://github.com/file/file.git BUILD
#build_fuzzer
set -x
cd BUILD
#libtoolize --force
#autoreconf -i
#./configure --enable-static
#make V=1 all

$CXX $CXXFLAGS -std=c++11 -Isrc/ \
	     $SCRIPT_DIR/magic_fuzzer.cc -o $OUT/magic_fuzzer \
	     ../libFuzzingEngine-libfuzzer.a ./src/.libs/libmagic.a -lz

cp ./magic/magic.mgc $OUT/
