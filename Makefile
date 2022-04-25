all: demix

# TODO: remove
.PHONY: demix

demix: src/demix.cpp
	@./scripts/check-dependencies.sh
	g++ -o demix -Wall -I./src/lib src/demix.cpp src/png-wrapper.cpp src/lib/bmplib.lib -lpng -lz
	@./scripts/rename.sh

# TODO:
# bmplib:
# 	@make -C lib/

clean:
#rm -rf $(OBJDIR)
	@rm -f *.exe
	@rm imgs/*-r.png && rm imgs/*-g.png && rm imgs/*-b.png


# TODO: -C option of make changes dir to path and runs make there
