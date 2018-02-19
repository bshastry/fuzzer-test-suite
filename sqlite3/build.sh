#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  mkdir BUILD
  cd BUILD
  export ASAN_OPTIONS=detect_leaks=0

  # Limit max length of data blobs and sql queries to prevent irrelevant OOMs.
  # Also limit max memory page count to avoid creating large databases.
  export CFLAGS="$CFLAGS -DSQLITE_MAX_LENGTH=128000000 \
               -DSQLITE_MAX_SQL_LENGTH=128000000 \
               -DSQLITE_MAX_MEMORY=25000000 \
               -DSQLITE_PRINTF_PRECISION_LIMIT=1048576 \
               -DSQLITE_DEBUG=1 \
               -DSQLITE_MAX_PAGE_COUNT=16384"             
               
  ../SRC/sqlite3/configure
  make -j $JOBS
  make sqlite3.c
  cd ..
}


fetch_src() {
  mkdir -p SRC/sqlite3 && \
    (cd SRC/sqlite3 && \
    curl 'https://www.sqlite.org/src/tarball?uuid=trunk' -o sqlite3.tar.gz && \
    tar xzf sqlite3.tar.gz)
}

set -x
fetch_src
build_lib
build_fuzzer
$CC $CFLAGS -I. -c \
    SRC/sqlite3/test/ossfuzz.c -o ossfuzz.o

$CXX $CXXFLAGS \
    ossfuzz.o -o $EXECUTABLE_NAME_BASE \
    $LIB_FUZZING_ENGINE BUILD/sqlite3.o

$CC $CFLAGS -c -g -I $LIBFUZZER_SRC/standalone $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c
$CC $CFLAGS -c -g -I $LIBFUZZER_SRC/standalone -I. SRC/sqlite3/test/ossfuzz.c -o ossfuzz-pf.o
$CXX $CXXFLAGS -g ossfuzz-pf.o StandaloneFuzzTargetMain.o BUILD/sqlite3.o -o $EXECUTABLE_NAME_BASE-pf
