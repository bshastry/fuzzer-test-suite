#!/bin/bash
# Copyright 2016 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

build_lib() {
  rm -rf BUILD
  cp -rf SRC BUILD
  (cd BUILD && find $SCRIPT_DIR -type f -name "*.patch" -print0 | xargs -0 -n1 -I{} bash -c "patch -p1 < {}" && libtoolize --force && ./boot.sh && ./configure && make -j $JOBS)
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
  nameobj=$name.o
  $CC $CFLAGS -c -g $SCRIPT_DIR/$1 -I BUILD -I BUILD/lib -I BUILD/include -I BUILD/ovn/controller
  $CXX $CXXFLAGS $nameobj BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic $LIB_FUZZING_ENGINE -o $EXECUTABLE_NAME_BASE_$name
#  $CXX $CXXFLAGS -DTRACKPF $nameobj StandaloneFuzzTargetMain.o -I $LIBFUZZER_SRC/standalone BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic  -o standalone_$name
  $CXX $CXXFLAGS $nameobj StandaloneFuzzTargetMain.o -I $LIBFUZZER_SRC/standalone BUILD/lib/.libs/libopenvswitch.a -lz -lssl -lcrypto -latomic  -o standalone_$name

}

#get_git_tag https://github.com/openvswitch/ovs.git v2.8.0 SRC
#build_lib
#build_fuzzer
#$CC $CFLAGS -DTRACKPF -c -g $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I $LIBFUZZER_SRC/standalone
$CC $CFLAGS -c -g $LIBFUZZER_SRC/standalone/StandaloneFuzzTargetMain.c -I $LIBFUZZER_SRC/standalone
set -x
#build_fuzz_target target.c
#build_fuzz_target target-ofpl.c
#build_fuzz_target target-ct.c
build_fuzz_target target-jsonp.c
#build_fuzz_target target-ofpd.c
#build_fuzz_target target-pinctrl.c
#build_cov
#build_cov_target target-ofpl.c

#build_fuzz_target target-odp.c
#build_fuzz_target target-odpact.c

#build_fuzz_target target-ofctl.c
#build_fuzz_target target-tlv-table-reply.c
#build_fuzz_target target-rconn.c
#build_fuzz_target target-ofpd.c
#build_fuzz_target target-ofpbuf.c
#build_fuzz_target target-odpid.c
#build_fuzz_target target-rtnlparse.c
#build_fuzz_target target-nxmatch.c
#build_fuzz_target target-ovsdbffstr.c
