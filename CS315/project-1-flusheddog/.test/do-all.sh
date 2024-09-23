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

Info "Report: `date`"

# Build the C Project ======================================
./do-build.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "application(s) did not build correctly. "
fi

# Run the C Project ========================================
./do-run.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "application(s) did not run correctly. "
fi

# Diff the C Project =======================================
./do-diff.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "application(s) did not diff correctly. "
fi

# Run Test =================================================
./do-test.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "Test application(s) had errors. "
fi

# Run CMake ================================================
./do-cmake.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "Test application(s) had CMake errors. "
fi

# Run Valgrind on the C Project ============================
./do-valgrind.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "application(s) had valgrind errors. "
fi

# Run Doxygen ==============================================
./do-doxygen.sh
ret=$?
if [ $ret -ne 0 ]; then
    Error "application(s) had doxygen errors. "
fi

# Print Report =============================================
cat "$REPORT_FILE"

# exit 1 if errors
if [ $errorCount -gt 0 ]; then
    exit 1
fi
