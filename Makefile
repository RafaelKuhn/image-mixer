# TODO: raw test this in actual linux distro
CC := g++

SRC_DIR := src/core
LIB_DIR := src/lib
OBJ_DIR := obj
INC_DIR := include

CPP_FLAGS =  -Wall
DEBUG_FLAGS = -Wall -Wextra -pedantic -D DEBUG_MODE

CORE_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.r.o, $(CORE_FILES))

LIBS := -lturbojpeg -lpng -lz

BMPLIB_PATH = $(LIB_DIR)/bmp-lib
BMPLIB = $(LIB_DIR)/bmplib.lib


DEBUG_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d.o, $(CORE_FILES))
BMPLIB_DEBUG = $(LIB_DIR)/bmplib_debug.lib


$(shell [ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) )

all: demix
debug: demix-debug

# compile demix program
demix: $(OBJS) $(BMPLIB) check-deps
	$(CC) -o demix.exe $(CPP_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/demix.cpp $(OBJS) $(BMPLIB) $(LIBS)
	@./scripts/rename-exe-windows.sh
demix-debug: $(DEBUG_OBJS) $(BMPLIB_DEBUG) check-deps
	$(CC) -o demix.exe $(DEBUG_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/demix.cpp $(DEBUG_OBJS) $(BMPLIB_DEBUG) $(LIBS)
	@./scripts/rename-exe-windows.sh


$(BMPLIB):
	make lib -C $(BMPLIB_PATH)
	cp $(BMPLIB_PATH)/lib/*.lib $(LIB_DIR)
$(BMPLIB_DEBUG):
	make lib-debug -C $(BMPLIB_PATH)
	cp $(BMPLIB_PATH)/lib/*.lib $(LIB_DIR)


# build objects for release targets (first deletes debug objects)
$(OBJ_DIR)/%.r.o: $(SRC_DIR)/%.cpp
	@rm -f $(DEBUG_OBJS)
	$(CC) $(CPP_FLAGS) -I$(INC_DIR) -c $< -o $@
# build objects for debug target (first deletes release objects)
$(OBJ_DIR)/%.d.o: $(SRC_DIR)/%.cpp
	@rm -f $(OBJS)
	$(CC) $(DEBUG_FLAGS) -I$(INC_DIR) -c $< -o $@


# hack to run dependency checking script only once
check-deps: scripts/DEPS_OK.tmp
	@:
scripts/DEPS_OK.tmp: scripts/check-dependencies.sh
	@./scripts/check-dependencies.sh 


clean:
	@make clean -C src/lib/bmp-lib/
	@rm -f $(OBJ_DIR)/*
	@rm -f demix*
	@rm -f mix*
	@rm -f scripts/DEPS_OK.tmp