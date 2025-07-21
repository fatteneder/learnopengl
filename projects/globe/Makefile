SRC =

###################################################

# https://stackoverflow.com/a/23324703
THISDIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

BIN = ./bin
$(shell mkdir -p $(BIN))

OBJ := $(SRC:cpp=o)
OBJ := $(SRC:c=o)
OBJ := $(addprefix $(BIN)/,$(OBJ))

CC = g++

CPPFLAGS = -g # -O3

# flags for extern libraries
OPENGL_FLAGS = $(shell pkgconf --cflags --libs opengl)
GLFW_FLAGS = $(shell pkgconf --cflags --libs glfw3)
GLM_FLAGS = $(shell pkgconf --cflags --libs glm)
EXT_LIB_FLAGS = $(CPPFLAGS) $(OPENGL_FLAGS) $(GLFW_FLAGS) $(GLM_FLAGS)

all: $(BIN)/main $(OBJ)

$(BIN)/%.o: %.cpp
	$(CC) $< -o $@ $(EXT_LIB_FLAGS)

GLAD_FLAGS = -I./glad/include
$(BIN)/glad.o: glad/src/glad.c
	$(CC) -c $< -o $@ $(EXT_LIB_FLAGS) $(GLAD_FLAGS)

$(BIN)/main: main.cpp shader.vs shader.fs camera.hpp $(OBJ) $(BIN)/glad.o
	@echo compiling main.cpp into $(BIN)/main
	$(CC) $< -o $@ -I. $(BIN)/glad.o $(EXT_LIB_FLAGS) $(GLAD_FLAGS) \
		-DSOURCE_DIR=$(THISDIR) \
		-DASSETS_DIR=$(THISDIR)../../assets/

clean:
	rm -rf $(BIN)

FORCE: ;
