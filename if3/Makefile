# Compiler and linker
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# SDL2, SDL2_ttf, and SDL2_image configuration
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_image

# Source files
SRCS = src/main.cpp src/storyNode.cpp src/commandParser.cpp src/commands.cpp

# Output executable
TARGET = interactive_fiction

# Build target
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -o $(TARGET) $(SRCS) $(SDL2_LDFLAGS) -Iinclude

# Clean target
clean:
	rm -f $(TARGET)

.PHONY: all clean
