CXX = g++
CXXFLAGS = -std=c++20 -O2 -Wno-unused-parameter

# Add -static to force static linking
LDFLAGS = -lstdc++ -lm
LDSTATIC = -lstdc++ -lm -static

# Directories
SRC_DIR = ./src
BUILD_DIR = ./build

# Find all source files recursively
SRCS = $(shell find $(SRC_DIR) -name "*.cpp")

# Generate corresponding object files in the build folder
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Name of the final executable
TARGET = tetroflow

# Default rule, first clean up the build and target, then build the target
all: compile

compile: $(TARGET)
# Rule to link object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)   # Create the directory for the object file if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

compile-static: $(TARGET)-static
# Rule to link object files into the final executable
$(TARGET)-static: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDSTATIC) -o $@ $^

# Rule to compile .cpp files into .o files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)   # Create the directory for the object file if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build and target
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets to avoid conflicts with files
.PHONY: all clean
