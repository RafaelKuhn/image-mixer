#!/bin/bash

set -x

# CC=g++
CC=clang++

CPP_FLAGS="-Wall"
DEBUG_FLAGS="--Wall -Wextra -pedantic -D DEBUG_MODE"

SRC_DIR="src/core"
INC_DIR="include"
OBJ_DIR="obj"

EXTERNAL_LIBS="-lturbojpeg -lpng -lzstd"

OBJS="";
for CPP_FILE in src/core/*.cpp; do
	BASE_NAME="${CPP_FILE%.cpp}"
	BASE_FILE_NAME=$(basename "$BASE_NAME");

	$CC -c -I "$INC_DIR" "$BASE_NAME.cpp" -o "obj/$BASE_FILE_NAME.o"
	OBJS="$OBJS obj/$BASE_FILE_NAME.o"
done

echo "################################################################"
echo "compiled objects: $OBJS"

# TODO: check status of submodule, clone it, init it, whatever
make -C "src/lib/bmp-lib"

# TODO: check if dependencies are installed

# TODO: grab programs from src/ use them to build the other ones, run chmod +x, etc

$CC -o demix "$CPP_FLAGS" -I "$SRC_DIR" -I "$INC_DIR" "src/demix.cpp" $OBJS "src/lib/bmp-lib/lib/bmplib.lib" $EXTERNAL_LIBS

./scripts/install.sh demix mix encode
./scripts/rename-exe-windows.sh demix mix encode

# TODO: figure out build for bmp monkey lib
# src/lib/bmp-lib/src/lib/bmplib.lib
