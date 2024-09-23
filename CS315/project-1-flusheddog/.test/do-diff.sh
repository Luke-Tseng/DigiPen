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

Info "Diff ==================================================="

# test output for each sample input/output file 
for i in {1..4}
do
    # Run C project with test $i
    echo "diff --strip-trailing-cr --ignore-space-change test-out/output-$i.txt $OUT/_output$i.txt 2>&1 | tee $OUT/_diff$i.txt"
    diff --strip-trailing-cr --ignore-space-change test-out/output-$i.txt $OUT/_output$i.txt 2>&1 | tee $OUT/_diff$i.txt
    if [ -s  $OUT/_diff$i.txt ]; then
        cat  $OUT/_diff$i.txt
        Error "Diff for case $i does not match."
    else
        Info "Diff for case $i matches correctly."
    fi
done

if [ $errorCount -gt 0 ]; then
    exit 1
fi
