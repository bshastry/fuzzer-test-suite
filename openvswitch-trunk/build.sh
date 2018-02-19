#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
#  (cd BUILD && find $SCRIPT_DIR -type f -name "*.patch" -print0 | xargs -0 -n1 -I{} bash -c "patch -p1 < {}" && libtoolize --force && ./boot.sh && ./configure && make -j $JOBS)
  (cd BUILD && libtoolize --force && ./boot.sh && ./configure && make -j $JOBS)
}

build_cov() {
  rm -rf COV
  cp -rf SRC COV
  (cd COV && libtoolize --force && ./boot.sh && CFLAGS="-fprofile-instr-generate -fcoverage-mapping" ./configure && make -j)
}

build_cov_target() {
  $CC -fprofile-instr-generate -fcoverage-mapping $SCRIPT_DIR/"$1" $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I COV -I COV/lib -I COV/include COV/lib/.libs/libopenvswitch.a -lz -lcrypto -lssl -lpthread -lrt -lm -o coverage-$(echo "$1" | sed 's/\.c//g')
}

build_fuzz_target() {
  name=$(echo "$1" | sed 's/\.c//g')
  addlib=$(find BUILD/ -type f -name "$2")
  nameobj=$name.o
  $CC $CFLAGS -c -g $SCRIPT_DIR/$1 -I BUILD -I BUILD/lib -I BUILD/include -I BUILD/ovn/controller
  $CXX $CXXFLAGS $nameobj  $addlib BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE_$name
#  $CXX $CXXFLAGS -DTRACKPF $nameobj StandaloneFuzzTargetMain.o -I $LIBFUZZER_SRC/standalone BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic  -o standalone_$name
  $CXX $CXXFLAGS $nameobj StandaloneFuzzTargetMain.o -I $LIBFUZZER_SRC/standalone $addlib BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic  -o standalone_$name
}


git clone https://github.com/openvswitch/ovs.git SRC
build_lib
build_fuzzer
#build_cov
set -x

### standalone stub
$CC $CFLAGS -c -g $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I $LIBFUZZER_SRC/standalone

### flow extract
#build_fuzz_target target.c

#### OF print
build_fuzz_target target-ofpbuf.c

#### CT
#build_fuzz_target target-ct.c

#### Ovn-lexer
#build_fuzz_target target-ovnlex.c libovn.a

#build_fuzz_target target-ovnexpr.c libovn.a

#### JSON
#build_fuzz_target target-jsonp.c

