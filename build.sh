#!/bin/bash

# set -x

# CC=g++
CC=clang++

CPP_FLAGS="-Wall"
DEBUG_FLAGS="--Wall -Wextra -pedantic -D DEBUG_MODE"

SRC_DIR="src/core"
INC_DIR="include"
OBJ_DIR="obj"

DST_DIR="bin"

EXTERNAL_LIBS="-lturbojpeg -lpng -lzstd"

# pacman -Syu
# pacman -S mingw-w64-x86_64-libjpeg-turbo
# pacman -S mingw-w64-x86_64-zlib
# pacman -S mingw64/mingw-w64-x86_64-libpng

# TODO: check if dependencies are installed

OBJS="";
for CPP_FILE in src/core/*.cpp; do
	BASE_PATH="${CPP_FILE%.cpp}"
	BASE_FILE_NAME=$(basename "$BASE_PATH");

	$CC -c -I "$INC_DIR" "$BASE_PATH.cpp" -o "obj/$BASE_FILE_NAME.o"
	OBJS="$OBJS obj/$BASE_FILE_NAME.o"
done

! [ -d "$DST_DIR" ] && {
	mkdir "$DST_DIR"
	echo "created folder: $DST_DIR"
}

echo "################################################################"
echo "compiled objects: $OBJS"

# TODO: check status of submodule, clone it, init it, whatever
make -C "src/lib/bmp-lib"

# TODO: grab programs from src/ use them to build the other ones, run chmod +x, etc

$CC -o "$DST_DIR"/demix "$CPP_FLAGS" -I "$SRC_DIR" -I "$INC_DIR" "src/demix.cpp" $OBJS "src/lib/bmp-lib/lib/bmplib.lib" $EXTERNAL_LIBS
$CC -o "$DST_DIR"/mix "$CPP_FLAGS" -I "$SRC_DIR" -I "$INC_DIR" "src/mix.cpp" $OBJS "src/lib/bmp-lib/lib/bmplib.lib" $EXTERNAL_LIBS
$CC -o "$DST_DIR"/encode "$CPP_FLAGS" -I "$SRC_DIR" -I "$INC_DIR" "src/encode.cpp" $OBJS "src/lib/bmp-lib/lib/bmplib.lib" $EXTERNAL_LIBS

export DST_DIR
./scripts/install.sh demix mix encode
./scripts/rename-exe-windows.sh demix mix encode

# TODO: figure out build for bmp monkey lib
# src/lib/bmp-lib/src/lib/bmplib.lib
