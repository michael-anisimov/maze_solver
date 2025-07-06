# Maze Pathfinding Visualizer Makefile
# Builds the organized project structure with clean separation of concerns

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -I./include
LIBS = -lncurses

# Directories
SRC_DIR = src
ALGOS_DIR = algos
UI_DIR = ui
UTILS_DIR = utils
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
MAIN_SRC = $(SRC_DIR)/main.cpp
ALGO_SOURCES = $(wildcard $(ALGOS_DIR)/*.cpp)
UI_SOURCES = $(wildcard $(UI_DIR)/*.cpp)
UTILS_SOURCES = $(wildcard $(UTILS_DIR)/*.cpp)

# All source files
SOURCES = $(MAIN_SRC) $(ALGO_SOURCES) $(UI_SOURCES) $(UTILS_SOURCES)

# Object files (in build directory)
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# Target executable
TARGET = maze_visualizer

# Default target
all: $(TARGET)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)
	mkdir -p $(BUILD_DIR)/$(ALGOS_DIR)
	mkdir -p $(BUILD_DIR)/$(UI_DIR)
	mkdir -p $(BUILD_DIR)/$(UTILS_DIR)

# Build the executable
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

# Compile source files to object files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Install dependencies (for macOS)
install-deps:
	brew install ncurses

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: $(TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build the application (default)"
	@echo "  clean        - Remove build files"
	@echo "  run          - Build and run the application"
	@echo "  debug        - Build with debug symbols"
	@echo "  release      - Build with optimizations"
	@echo "  install-deps - Install dependencies (macOS)"
	@echo "  docs         - Show project documentation"
	@echo "  help         - Show this help message"

.PHONY: all clean install-deps run debug release docs help 