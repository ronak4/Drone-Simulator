OBJS = src/main.o src/Camera.o src/Drone.o src/InputHandler.o src/Scene.o src/Shader.o

INCLUDES = -Iinclude -I../include

LIBS = -L../lib

LDFLAGS = -lglad -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit

CFLAGS = -g -std=c++17

PROGRAM = drone

ifeq ($(OS),Windows_NT)
    LDFLAGS += -lopengl32 -lgdi32
    PROGRAM := $(addsuffix .exe, $(PROGRAM))
    COMPILER = g++
else ifeq ($(shell uname -s), Darwin)
    COMPILER = clang++
else
    COMPILER = g++
endif

$(PROGRAM): $(OBJS)
	$(COMPILER) -o $(PROGRAM) $(OBJS) $(LIBS) $(LDFLAGS)

src/%.o: src/%.cpp
	$(COMPILER) $(INCLUDES) $(CFLAGS) -c $< -o $@

RM = rm -f
ifeq ($(OS),Windows_NT)
    RM = del
endif

clean:
	$(RM) $(OBJS) $(PROGRAM)

.PHONY: clean
