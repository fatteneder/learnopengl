SRC =

###################################################

BIN = ./bin
$(shell mkdir -p $(BIN))

OBJ := $(SRC:cpp=o)
OBJ := $(SRC:c=o)
OBJ := $(addprefix $(BIN)/,$(OBJ))

CC = g++

CPPFLAGS = -g # -OE
OPENGL_FLAGS = $(shell pkgconf --cflags --libs opengl)
GLFW_FLAGS = $(shell pkgconf --cflags --libs glfw3)
GL_FLAGS = $(shell pkgconf --cflags --libs gl)
FLAGS = $(CPPFLAGS) $(OPENGL_FLAGS) $(GLFW_FLAGS) $(GL_FLAGS)

all: $(BIN)/main $(OBJ)

$(BIN)/%.o: %.cpp
	$(CC) $< -o $@ $(FLAGS)

GLAD_FLAGS = -I./glad/include
$(BIN)/glad.o: glad/src/glad.c
	$(CC) -c $< -o $@ $(FLAGS) $(GLAD_FLAGS)

# make chapter_X/Y to compile that into main
CHAPS := $(shell ls -d chapter_*/*)
$(CHAPS) : chapter_% : chapter_%/main.cpp $(OBJ) $(BIN)/glad.o
	@echo compiling $@ into $(BIN)/main
	$(CC) $@/main.cpp -o $(BIN)/main $(BIN)/glad.o $(FLAGS) $(GLAD_FLAGS)

$(BIN)/main: main.cpp $(OBJ) $(BIN)/glad.o
	$(CC) $< -o $@ $(BIN)/glad.o $(FLAGS) $(GLAD_FLAGS)

clean:
	rm -rf $(BIN)
