#!/bin/sh

printf "[shell] checking if dependencies are installed\n"

[ "$(gcc -lturbojpeg 2>&1 | grep "cannot find")" ] && {
	printf "[shell] error: cannot find libturbojpeg (gcc -lturbojpeg)\n"
	exit 1
}

[ "$(gcc -lpng 2>&1 | grep "cannot find")" ] && {
	printf "[shell] error: cannot find libpng (gcc -lpng)\n"
	exit 1
}

[ "$(gcc -lzstd 2>&1 | grep "cannot find")" ] && {
	printf "[shell] error: cannot find zlib (gcc -lzstd)\n"
	exit 1
}

[ "$(ls -A src/lib/bmp-lib)" ] || {
	printf "[shell] error: bmp-lib submodule not found! try running \n\44 git submodule update --init --recursive\n"
	exit 1
}

printf "[shell] all dependencies are installed!\n"
[ "$1" = "" ] || {
	printf "\-lturbojpeg \-lpng \-lzstd" > "$1"
}