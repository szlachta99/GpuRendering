# Compiler and flags
CC = g++
CDEBUGFLAGS = -fdiagnostics-color=always -g -DDEBUG
CFLAGS = -Wall -Iinclude -std=c++17 $(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3) -lm

SC = glslc

# Directories
SRC_DIR = src
BUILD_DIR = build
EXECUTABLE = GPU
SHADER_SRC_DIR = shaders/src
SHADER_BUILD_DIR = shaders/bin

# Find all source files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Collect all shader targets
VERTEX_SHADERS = $(wildcard $(SHADER_SRC_DIR)/*.glsl.vert)
FRAGMENT_SHADERS = $(wildcard $(SHADER_SRC_DIR)/*.glsl.frag)

SHADER_TARGETS = $(patsubst $(SHADER_SRC_DIR)/%.glsl.vert, $(SHADER_BUILD_DIR)/%.spv.vert, $(VERTEX_SHADERS)) \
                 $(patsubst $(SHADER_SRC_DIR)/%.glsl.frag, $(SHADER_BUILD_DIR)/%.spv.frag, $(FRAGMENT_SHADERS))

# Default target
all: $(EXECUTABLE) $(SHADER_TARGETS)

# Link the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile shaders
$(SHADER_BUILD_DIR)/%.spv.vert: $(SHADER_SRC_DIR)/%.glsl.vert | $(SHADER_BUILD_DIR)
	$(SC) $< -o $@

$(SHADER_BUILD_DIR)/%.spv.frag: $(SHADER_SRC_DIR)/%.glsl.frag | $(SHADER_BUILD_DIR)
	$(SC) $< -o $@

# Create build directories if they don't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SHADER_BUILD_DIR):
	mkdir -p $(SHADER_BUILD_DIR)

debug: CFLAGS += $(CDEBUGFLAGS)
debug: all

run: all
	./$(EXECUTABLE)

# Clean the build directory and executable
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE) $(SHADER_BUILD_DIR)

.PHONY: all clean run
