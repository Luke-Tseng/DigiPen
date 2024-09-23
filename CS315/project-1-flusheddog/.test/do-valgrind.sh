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

Info "Valgrind ==============================================="

OUTFILE="$OUT/valgrind.txt"
# Run Valgrind on C project
echo "valgrind -q --leak-check=full --show-reachable=yes --tool=memcheck $BIN/$EXE >/dev/null 2> $OUTFILE"
valgrind -q --leak-check=full --show-reachable=yes --tool=memcheck $BIN/$EXE >/dev/null 2> "$OUTFILE"

ret=$?
if [ $ret -ne 0 ]; then
    cat "$OUTFILE"
    Error "Application did not execute correctly during run 'valgrind bin/$EXE'; Probably a crash."
else
    Info "'valgrind bin/$EXE' executed."
fi

leaksCount=$(grep -c -i "blocks are definitely " "$OUTFILE")
if [ $leaksCount -gt 0 ]; then
    cat "$OUTFILE"
    Error "$(grep -i -A 2 "blocks are definitely " "$OUTFILE")"
    Error "$leaksCount leak(s) found"
else
    Info "Found no leaks."
fi

if [ $errorCount -gt 0 ]; then
    exit 1
fi
