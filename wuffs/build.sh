#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

git clone --depth 1 https://github.com/google/wuffs.git BUILD
build_fuzzer
set -x
OUT=..
cd BUILD
for f in fuzz/c/std/*_fuzzer.cc; do
  b=$(basename $f _fuzzer.cc)
  $CXX $CXXFLAGS -std=c++11 $f -o $OUT/${b}_fuzzer $OUT/$LIB_FUZZING_ENGINE
  mkdir $OUT/seed_corpus_${b} && cp test/testdata/*.$b $OUT/seed_corpus_${b}
done
