#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

git clone --depth 1 git://git.sv.nongnu.org/freetype/freetype2.git BUILD
build_fuzzer
set -x
OUT=..
cd BUILD
sed -i 's/run aclocal/run \$HOME\/local\/bin\/aclocal/' autogen.sh
./autogen.sh
sync
./configure
make -j$(nproc) clean
make -j$(nproc) all

$CXX $CXXFLAGS -std=c++11 \
	  -I./include -I. -I$HOME/local/include \
	    ./src/tools/ftfuzzer/ftfuzzer.cc -o $OUT/ftfuzzer \
	      ./objs/*.o ../$LIB_FUZZING_ENGINE \
		/home/users/bshastry/local/lib/libarchive.a \
		  ./objs/.libs/libfreetype.a \
		  -lpng -lz -lbz2 -lharfbuzz

##zip -j $OUT/ftfuzzer_seed_corpus.zip $SRC/font-corpus/*
mkdir -p $OUT/seed_corpus
wget -P $OUT/seed_corpus https://github.com/adobe-fonts/adobe-variable-font-prototype/releases/download/1.001/AdobeVFPrototype.otf
git clone https://github.com/unicode-org/text-rendering-tests.git && cp text-rendering-tests/fonts/* $OUT/seed_corpus
