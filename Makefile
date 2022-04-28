BMPLIB_PATH=src/lib/bmp-lib

DEBUG_FLAGS = -Wall -Wextra -pedantic -D DEBUG_MODE

all: demix

# TODO: remove phony and make a decent makefile

debug: demix-debug

demix: src/demix.cpp
	@./scripts/check-dependencies.sh
# compile bmplib
	@[ -d src/lib/bmp-lib ] || printf "[make] you must have the bmp-lib submodule at src/lib/bmp-lib folder, try running\n\44 git submodule update --init --recursive\n";
	make lib -C src/lib/bmp-lib/
	@cp src/lib/bmp-lib/lib/* src/lib/
# compile demix
	g++ -o demix -Wall -I./src/lib src/demix.cpp src/timer.cpp src/jpeg-wrapper.cpp src/png-wrapper.cpp src/lib/bmplib.lib -lturbojpeg -lpng -lz
	@./scripts/rename-exe-windows.sh

demix-debug: src/demix.cpp
	@./scripts/check-dependencies.sh
# compile bmplib
	@[ -d src/lib/bmp-lib ] || printf "[make] you must have the bmp-lib submodule at src/lib/bmp-lib folder, try running\n\44 git submodule update --init --recursive\n";
	make lib-debug -C src/lib/bmp-lib
	@cp src/lib/bmp-lib/lib/* src/lib/
# compile demix
	g++ $(DEBUG_FLAGS) -o demix -Wall -I./src/lib src/demix.cpp src/timer.cpp src/jpeg-wrapper.cpp src/png-wrapper.cpp src/lib/bmplib_debug.lib -lturbojpeg -lpng -lz
	@./scripts/rename-exe-windows.sh

# bmplib:
# # make lib -C src/lib/bmp-lib
	

# bmplib-debug:
	



.PHONY: demix



# TODO: use make to recompile bmplib so it works across platforms instead of adding bmplib.lib to git
# bmplib:
# 	@make -C lib/bmplib

clean:
	@make clean -C src/lib/bmp-lib
	@rm -f demix*
	@rm -f *.exe
	@rm -f imgs/*-r.png && rm -f imgs/*-g.png && rm -f imgs/*-b.png
