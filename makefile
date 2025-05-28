# Compiler and flags
CXX := g++
# Enable debug symbols (-g) and disable optimization (-O0)
CXXFLAGS := -std=c++17 -Wall -g -O0 -Iinclude -Iinclude/Roles
# Linker flags for SFML (used by both app and tests)
LDFLAGS := -lsfml-graphics -lsfml-window -lsfml-system

# Directory definitions
SRC_DIR := src
ROLES_DIR := $(SRC_DIR)/Roles
GUI_DIR := $(SRC_DIR)/GUI
OBJ_DIR := obj

# Gather all source filenames
SRC_SRCS   := $(wildcard $(SRC_DIR)/*.cpp)
ROLES_SRCS := $(wildcard $(ROLES_DIR)/*.cpp)
GUI_SRCS   := $(wildcard $(GUI_DIR)/*.cpp)

# Object files for full application (includes GUI & main)
OBJS_APP := \
	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_SRCS)) \
	$(patsubst $(ROLES_DIR)/%.cpp,$(OBJ_DIR)/Roles_%.o,$(ROLES_SRCS)) \
	$(patsubst $(GUI_DIR)/%.cpp,$(OBJ_DIR)/GUI_%.o,$(GUI_SRCS))

# Test object file (tests.cpp)
OBJ_TEST := $(OBJ_DIR)/tests.o

# Executable names
TARGET      := main
TEST_RUNNER := test_runner

# Default target: build full application
all: $(TARGET)

# Build and run application
run: $(TARGET)
	@echo "Running application..."
	./$(TARGET)

# Build test runner (excludes main.o)
$(TEST_RUNNER): $(filter-out $(OBJ_DIR)/main.o,$(OBJS_APP)) $(OBJ_TEST)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile and run unit tests
test: $(TEST_RUNNER)
	@echo "Running unit tests..."
	./$(TEST_RUNNER)

# Compile tests.cpp into object file
$(OBJ_DIR)/tests.o: tests.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generic compile rule for src/ files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generic compile rule for Roles/ files
$(OBJ_DIR)/Roles_%.o: $(ROLES_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generic compile rule for GUI/ files
$(OBJ_DIR)/GUI_%.o: $(GUI_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link full application with SFML
$(TARGET): $(OBJS_APP)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Clean up build artifacts
.PHONY: all run test clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(TEST_RUNNER)
