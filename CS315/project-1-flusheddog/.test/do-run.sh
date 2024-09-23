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

Info "Run ===================================================="

# Run C project
for i in {1..4}
do
    # Run C project with test $i
    echo "$BIN/$EXE $i 2>&1 | tee $OUT/_output$i.txt"
    $BIN/$EXE $i 2>&1 | tee $OUT/_output$i.txt

    ret=$?
    if [ $ret -ne 0 ]; then
        cat $OUT/_output$i.txt
        Error "'bin/$EXE' did not execute correctly. Probably a crash."
    else
        Info "'bin/$EXE' executed."
    fi

    if [ ! -f "$OUT/_output$i.txt" ]; then
        Error "'bin/$EXE' did not execute correctly. 'out/_output$i.txt' does not exist."
    fi
done

if [ $errorCount -gt 0 ]; then
    exit 1
fi
