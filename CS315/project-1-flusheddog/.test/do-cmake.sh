#!/bin/bash

# Setup ====================================================

# save current working directory 
baseDir="$(pwd)"
# save directory contining this script
gradingDir="$(realpath "$(dirname "$0")")"

# Check the working directory against the ./.test directory
#  Change directory if needed to make pathing consistent
if [ "$baseDir" != "$gradingDir" ];
then
    cd "$gradingDir"
fi

source shell/config.sh

Info "CMake =================================================="

cd ..

# CMake to generate Unix Makefiles using current source directory and creating a new build directory
# - This will automatically download the GoogleTest and/or Catch2 dependency library
OUTFILE="out/cmake.txt"
echo "cmake -G \"Unix Makefiles\" -S . -Bbuild-linux 2>&1 | tee $OUTFILE"
cmake -G "Unix Makefiles" -S . -Bbuild-linux 2>&1 | tee "$OUTFILE"
errorCount=$(grep -c "error" "$OUTFILE")
if [ $errorCount -gt 0 ]; then
    cat "$OUTFILE"
    Error "$errorCount cmake errors found"
else
    Info "CMake files generated."
fi

# check if 'build-linux' directory exists
if [ ! -d "build-linux" ]; then
    cat "$OUTFILE"
    Error "CMake did not output the build-linux directory."
else
    Info "CMake files built to 'build-linux' directory."
fi

# Use CMake to build-linux the project 
echo "cmake --build build-linux/ 2>&1 | tee -a $OUTFILE"
cmake --build build-linux/ 2>&1 | tee -a "$OUTFILE"
cmakeBuildTestErrorCount=$(grep -c "error" "$OUTFILE")
if [ $cmakeBuildTestErrorCount -gt 0 ]; then
    cat "$OUTFILE"
    Error "$cmakeBuildTestErrorCount cmake build errors found"
else
    Info "CMake build executed."
fi

echo "cd build-linux/src/Test"
cd build-linux/src/Test

OUTFILE="../../$OUT/ctest.txt"
# Use CTest to run our tests
echo "ctest -V --timeout 100 2>&1 | tee $OUTFILE"
ctest -V --timeout 100 2>&1 | tee "$OUTFILE"
ctestErrorCount=$(grep -c "failed" "$OUTFILE")
if [ $ctestErrorCount -gt 1 ]; then
    cat "$OUTFILE"
    Error "$ctestErrorCount ctest errors found"
else
    Info "CTest executed."
fi

echo "cd ../../../.test/"
cd ../../../.test/

if [ $errorCount -gt 0 ]; then
    exit 1
fi
