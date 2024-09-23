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

Info "Test ==================================================="

OUTFILE="$OUT/$TEST_EXE-all.txt"
# Run Test C++ project
echo "bin/$TEST_EXE 2>&1 | tee $OUTFILE"
$BIN/$TEST_EXE 2>&1 | tee "$OUTFILE"
if [ ! -f "$OUTFILE" ]; then
    Error "'bin/$TEST_EXE' did not execute correctly. $OUTFILE does not exist."
else
    Info "'bin/$TEST_EXE' executed."
fi

# Check Test C++ project for failed test cases
#   search for 'failed' and count failures
failedTestCount=$(grep -c "failed" "$OUTFILE")
if [ $failedTestCount -gt 0 ]; then
    cat "$OUTFILE"
    Error "$failedTestCount test(s) failed."
else
    Info "All tests passed."
fi

if [ $errorCount -gt 0 ]; then
    exit 1
fi
