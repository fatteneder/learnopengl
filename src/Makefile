# https://stackoverflow.com/a/23324703
THISDIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

BIN = ./bin
$(shell mkdir -p $(BIN))

# SRC =
# OBJ := $(SRC:cpp=o)
# OBJ := $(SRC:c=o)
# OBJ := $(addprefix $(BIN)/,$(OBJ))

ifeq (,$(MAKECMDGOALS))
SHADERS = $(wildcard *.vs) $(wildcard *.fs)
else
SHADERS = $(wildcard $(MAKECMDGOALS)/*.vs) $(wildcard $(MAKECMDGOALS)/*.vs)
endif

CC = g++

CPPFLAGS = -g # -O3

# flags for extern libraries
OPENGL_FLAGS = $(shell pkgconf --cflags --libs opengl)
GLFW_FLAGS = $(shell pkgconf --cflags --libs glfw3)
GLM_FLAGS = $(shell pkgconf --cflags --libs glm)
EXT_LIB_FLAGS = $(CPPFLAGS) $(OPENGL_FLAGS) $(GLFW_FLAGS) $(GLM_FLAGS)

all: $(BIN)/main #$(OBJ)

$(BIN)/%.o: %.cpp
	$(CC) $< -o $@ $(EXT_LIB_FLAGS)

GLAD_FLAGS = -I./glad/include
$(BIN)/glad.o: glad/src/glad.c
	$(CC) -c $< -o $@ $(EXT_LIB_FLAGS) $(GLAD_FLAGS)

# use `make chapter_X/Y` to compile the chapter_X/Y/main.cpp into main
# use a static pattern rule to limit consideration of targets: https://stackoverflow.com/a/15718720
# CHAPS := $(shell ls -d chapter_*/*)
CHAPS := $(wildcard chapter_*/*)
$(CHAPS) : chapter_% : FORCE chapter_%/main.cpp $(SHADERS) $(BIN)/glad.o # $(OBJ)
	@echo compiling $@ into $(BIN)/main
	$(CC) $@/main.cpp -o $(BIN)/main -I. $(BIN)/glad.o $(EXT_LIB_FLAGS) $(GLAD_FLAGS) \
		-DSOURCE_DIR=$(THISDIR)$@ \
		-DASSETS_DIR=$(THISDIR)../assets/

$(BIN)/main: main.cpp $(SHADERS) $(BIN)/glad.o # $(OBJ)
	@echo compiling main.cpp into $(BIN)/main
	$(CC) $< -o $@ -I. $(BIN)/glad.o $(EXT_LIB_FLAGS) $(GLAD_FLAGS) \
		-DSOURCE_DIR=$(THISDIR) \
		-DASSETS_DIR=$(THISDIR)../assets/

clean:
	rm -rf $(BIN)

FORCE: ;
