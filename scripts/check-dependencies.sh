#!/bin/bash

echo "[shell] checking if dependencies are installed"

[ "$(gcc -lturbojpeg 2>&1 | grep "cannot find")" ] && {
	echo "[shell] error: cannot find libturbojpeg (gcc -lturbojpeg)"
	exit 1
}

[ "$(gcc -lpng 2>&1 | grep "cannot find")" ] && {
	echo "[shell] error: cannot find libpng (gcc -lpng)"
	exit 1
}

[ "$(gcc -lz 2>&1 | grep "cannot find")" ] && {
	echo "[shell] error: cannot find zlib (gcc -lz)"
	exit 1
}

[ -d src/lib/bmp-lib ] || {
	printf "[shell] error: bmp-lib submodule not found! try running \n\44 git submodule update --init --recursive\n"
	exit 1
}

[ -f src/lib/bmplib.lib ] || {
	printf "[shell] error: bmplib is not compiled (src/lib/bmplib.lib) try running\n\44 make bmplib\n"
	exit 1
}

echo "[shell] all dependencies are installed!"