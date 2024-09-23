#!/bin/bash

# Setup ====================================================

# save current working directory 
baseDir="$(pwd)"
# save directory contining this script
gradingDir="$(realpath "$(dirname "$0")")"
# save base project directory without path
folderName="$(basename "$(pwd)")"

# Check the working directory against the ./.test directory
#  Change directory if needed to make pathing consistent
if [ "$baseDir" != "$gradingDir" ];
then
    cd "$gradingDir"
fi

source shell/config.sh

Info "Doxygen ================================================"

cd .. # cd out of .test/

OUTFILE="out/doxygen.txt"
# Run Doxygen on the source files
echo "PROJECT_NAME=$folderName doxygen 2>&1 | tee $OUTFILE"
PROJECT_NAME=$folderName doxygen 2>&1 | tee "$OUTFILE"
if [ -s "$OUTFILE" ]; then
    cat "$OUTFILE"
    warningCount=$(grep -c -i "warning:" "$OUTFILE")
    if [ $warningCount -gt 0 ]; then
        Warn "$(grep -i "warning" "$OUTFILE")"
        Warn "$warningCount doxygen warnings"
    fi

    errorCount=$(grep -c -i "error:" "$OUTFILE")
    if [ $errorCount -gt 0 ]; then
        Error "$(grep -i "error" "$OUTFILE")"
        Error "$errorCount doxygen errors"
    fi
fi

# Verify Doxygen ran correctly (if 'out/latex' directory exists)
if [ ! -d "out/latex" ]; then
    cat "$OUTFILE"
    Error "Doxygen did not create the 'out/latex' directory."
else
    Info "doxygen output 'out/latex' directory."
fi

cd out/latex

OUTFILE="../doxygen-latex.txt"
# Create refman.pdf 
echo "make 2>&1 | tee $OUTFILE"
make 2>&1 | tee "$OUTFILE"

# Verify 'refman.pdf' exists
if [ ! -f "refman.pdf" ]; then
    cat "$OUTFILE"
    Error "latex did not output the expected \"refman.pdf\" file."
else
    echo "cp refman.pdf ../$DOCS/refman.pdf 2>&1 | tee -a $OUTFILE"
    cp refman.pdf ../$DOCS/refman.pdf 2>&1 | tee -a "$OUTFILE"
    Info "doxygen-latex output \"docs/refman.pdf\" correctly."
fi

cd ../../.test

if [ $errorCount -gt 0 ]; then
    exit 1
fi
