CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2 -Wunused-parameter

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build

# Find all source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Generate corresponding object files in the build folder
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Name of the final executable
TARGET = app

# Default rule
all: $(TARGET)

# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean up the build and target
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets to avoid conflicts with files
.PHONY: all clean
