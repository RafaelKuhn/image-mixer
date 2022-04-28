all: demix

# TODO: remove phony and make a decent makefile
.PHONY: demix

demix: src/demix.cpp
# @./scripts/check-dependencies.sh
	g++ -o demix -Wall -I./src/lib src/demix.cpp src/timer.cpp src/jpeg-wrapper.cpp src/png-wrapper.cpp src/lib/bmplib.lib -lturbojpeg -lpng -lz
	@./scripts/rename-exe-windows.sh

# TODO: use make to recompile bmplib so it works across platforms instead of adding bmplib.lib to git
# bmplib:
# 	@make -C lib/bmplib

clean:
	@rm -f *.exe
	@rm imgs/*-r.png && rm imgs/*-g.png && rm imgs/*-b.png
