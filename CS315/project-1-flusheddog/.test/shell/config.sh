# Setup ====================================================

# set our compilers CC is the common C compiler variable name, CXX is the common C++ compiler variable name
CC="clang"
CXX="clang++"

EXE=intro-project
TEST_EXE=$EXE-test

SRC=../src
OUT=../out
BUILD=../build
BIN=../bin
DOCS=../docs

# Create the output directories if they do not exist
mkdir -p $OUT
mkdir -p $BIN
mkdir -p $DOCS
mkdir -p $BUILD

REPORT_FILE="$gradingDir/$OUT/report.txt"
errorCount=0

# Print colored output
source shell/colors

REPORT_FILE="$gradingDir/$OUT/report.txt"
errorCount=0
warningCount=0
infoCount=0
logCount=0

function Error {
    printRed "$1"
    echo "$1" >> "$REPORT_FILE"
    ((errorCount++))
}
function Warn {
    printOrange "$1"
    echo "$1" >> "$REPORT_FILE"
    ((warningCount++))
}
function Info {
    printCyan "$1"
    echo "$1" >> "$REPORT_FILE"
    ((infoCount++))
}
function Log {
    printGray "$1"
    echo "$1" >> "$REPORT_FILE"
    ((logCount++))
}
