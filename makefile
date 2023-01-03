CC=g++
LIB=-lGLEW -lGL -lGLU -lglfw -lm -ldl
CFLAGS=-Wall -Wextra -Werror -std=c++17 -O3

all: Main

Main: src/*.cpp
	$(CC) $(CFLAGS) -o Main src/*.cpp src/vendor/imgui/*.cpp src/vendor/stb_image/*.cpp $(LIB)

clean:
	rm -f Main