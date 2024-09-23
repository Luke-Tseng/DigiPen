#!/usr/bin/env bash

BUILD_CONFIG=${1:-Debug}
BUILD_DIR=${2:-build-linux}
CXX=${3:-clang++}

mkdir -p $BUILD_DIR

echo "cmake -G \"Unix Makefiles\" -DCMAKE_BUILD_TYPE=$BUILD_CONFIG -S . -B$BUILD_DIR"
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$BUILD_CONFIG -S . -B$BUILD_DIR 

echo "cmake --build ./$BUILD_DIR --config $BUILD_CONFIG"
cmake --build ./$BUILD_DIR --config $BUILD_CONFIG
s