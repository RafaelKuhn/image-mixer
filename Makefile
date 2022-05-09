# TODO: raw test this in actual linux distro
CC := g++

SRC_DIR := src/core
LIB_DIR := src/lib
OBJ_DIR := obj
INC_DIR := include

CPP_FLAGS = -Wall
DEBUG_FLAGS = -Wall -Wextra -pedantic -D DEBUG_MODE

# TODO: wildcard "core main files"

CORE_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.r.o, $(CORE_FILES))
DEBUG_OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.d.o, $(CORE_FILES))

LIBS := -lturbojpeg -lpng -lzstd

BMPLIB_PATH = $(LIB_DIR)/bmp-lib
BMPLIB = $(LIB_DIR)/bmplib.lib
BMPLIB_DEBUG = $(LIB_DIR)/bmplib_debug.lib

DEPS_OK_FILE = scripts/DEPS_OK.tmp


# creates obj dir, since git doesn't track files and file names, but contents
$(shell [ -d $(OBJ_DIR) ] || mkdir $(OBJ_DIR) )

all: demix mix encode
debug: demix-debug mix-debug encode-debug

# TODO: make install that depends on all

# compile demix program
demix: $(DEPS_OK_FILE) $(OBJS) $(BMPLIB) src/demix.cpp
	$(CC) -o demix.exe $(CPP_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/demix.cpp $(OBJS) $(BMPLIB) $(LIBS)
	@./scripts/rename-exe-windows.sh demix
demix-debug: $(DEPS_OK_FILE) $(DEBUG_OBJS) $(BMPLIB_DEBUG) src/demix.cpp
	$(CC) -o demix.exe $(DEBUG_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/demix.cpp $(DEBUG_OBJS) $(BMPLIB_DEBUG) $(LIBS)
	@./scripts/rename-exe-windows.sh demix

# compile mix program
mix: $(DEPS_OK_FILE) $(OBJS) $(BMPLIB) src/mix.cpp
	$(CC) -o mix.exe $(CPP_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/mix.cpp $(OBJS) $(BMPLIB) $(LIBS)
	@./scripts/rename-exe-windows.sh mix
mix-debug: $(DEPS_OK_FILE) $(DEBUG_OBJS) $(BMPLIB_DEBUG) src/mix.cpp
	$(CC) -o mix.exe $(DEBUG_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/mix.cpp $(DEBUG_OBJS) $(BMPLIB_DEBUG) $(LIBS)
	@./scripts/rename-exe-windows.sh mix

# compile encode program
encode: $(DEPS_OK_FILE) $(OBJS) $(BMPLIB) src/encode.cpp
	$(CC) -o encode.exe $(CPP_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/encode.cpp $(OBJS) $(BMPLIB) $(LIBS)
	@./scripts/rename-exe-windows.sh encode
encode-debug: $(DEPS_OK_FILE) $(DEBUG_OBJS) $(BMPLIB_DEBUG) src/encode.cpp
	$(CC) -o encode.exe $(DEBUG_FLAGS) -I$(SRC_DIR) -I$(INC_DIR) src/encode.cpp $(DEBUG_OBJS) $(BMPLIB_DEBUG) $(LIBS)
	@./scripts/rename-exe-windows.sh encode


# compile included library bmplib
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


# hack to run dependency checking script until it creates the file $(DEPS_OK_FILE)
$(DEPS_OK_FILE): scripts/check-dependencies.sh
	@./scripts/check-dependencies.sh $(DEPS_OK_FILE)

install: all
	@./scripts/install.sh mix demix encode

uninstall:
	@./scripts/uninstall.sh mix demix encode


clean:
	@make clean -C src/lib/bmp-lib/
	@rm -f $(BMPLIB)
	@rm -f $(BMPLIB_DEBUG)
	@rm -f $(OBJ_DIR)/*
	@rm -f demix*
	@rm -f mix*
	@rm -f scripts/DEPS_OK.tmp
