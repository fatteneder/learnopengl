SRC =

###################################################

BIN = ./bin
$(shell mkdir -p $(BIN))

OBJ := $(SRC:cpp=o)
OBJ := $(addprefix $(BIN),$(OBJ))

CC = g++

CPPFLAGS = -O3
OPENGL_FLAGS = $(shell pkgconf --cflags --libs opengl)
GLFW_FLAGS = $(shell pkgconf --cflags --libs glfw3)
GL_FLAGS = $(shell pkgconf --cflags --libs gl)
FLAGS = $(CPPFLAGS) $(OPENGL_FLAGS) $(GLFW_FLAGS) $(GL_FLAGS)

all: $(BIN)/main $(OBJ)

$(BIN)/%.o: $%.cpp
	$(CC) $< -o $@ $(FLAGS)

$(BIN)/main: main.cpp
	$(CC) $< -o $@ $(FLAGS)

clean:
	rm -rf $(BIN)
