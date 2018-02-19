#!/bin/bash
# Copyright 2017 Google Inc. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
. $(dirname $0)/../common.sh

git clone --depth 1 https://github.com/bblanchon/ArduinoJson.git BUILD
build_fuzzer
set -x
$CXX $CXXFLAGS -I BUILD BUILD/fuzzing/fuzzer.cpp $(find BUILD/src -type f) $LIB_FUZZING_ENGINE
