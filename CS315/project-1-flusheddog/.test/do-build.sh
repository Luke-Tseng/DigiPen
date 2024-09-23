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

# Input shell/config.sh
source shell/config.sh

Info "Build =================================================="

OUTFILE="$gradingDir/$OUT/$CC-$EXE.txt"
# Compile C++ project
Log "$CC -Wall -Wextra -Werror -std=c++17 -O2 -Wno-unused-result -D_XOPEN_SOURCE $SRC/SimpleMath/SimpleMath.cpp $SRC/MockExecutable/main.cpp -I$SRC/SimpleMath/include -o $BIN/$EXE 2>&1 | tee $OUTFILE"
$CC -Wall -Wextra -Werror -std=c++17 -O2 -Wno-unused-result -D_XOPEN_SOURCE $SRC/SimpleMath/SimpleMath.cpp $SRC/MockExecutable/main.cpp -I$SRC/SimpleMath/include -o $BIN/$EXE 2>&1 | tee "$OUTFILE"
# check if '$BIN/$EXE' exists
if [ ! -f "$BIN/$EXE" ]; then
    cat "$OUTFILE"
    Error "bin/$EXE did not compile."
else
    Info "'bin/$EXE' compiled."
fi

OUTFILE="$gradingDir/$OUT/$CXX-$TEST_EXE.txt"
# Compile test project
Log "$CXX -Wall -Wextra -Werror -std=c++17 -x c++ -O2 -Wno-unused-result -D_XOPEN_SOURCE $SRC/SimpleMath/SimpleMath.cpp $SRC/Test/SimpleMathTests.cpp $SRC/Test/infrastructure/infrastructure.cpp -DENABLE_CATCH2TEST -I$SRC/SimpleMath/include -o $BIN/$TEST_EXE 2>&1 | tee $OUTFILE"
$CXX -Wall -Wextra -Werror -std=c++17 -x c++ -O2 -Wno-unused-result -D_XOPEN_SOURCE $SRC/SimpleMath/SimpleMath.cpp $SRC/Test/SimpleMathTests.cpp $SRC/Test/infrastructure/infrastructure.cpp -DENABLE_CATCH2TEST -I$SRC/SimpleMath/include -o $BIN/$TEST_EXE 2>&1 | tee "$OUTFILE"
# check if '$BIN/$TEST_EXE' exists
if [ ! -f "$BIN/$TEST_EXE" ]; then
    cat "$OUTFILE"
    Error "bin/$TEST_EXE did not compile."
else
    Info "'bin/$TEST_EXE' compiled."
fi

if [ $errorCount -gt 0 ]; then
    exit 1
fi
