# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinclude/Roles -Iinclude/GUI
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Directory structure
SRC_DIR = src
ROLES_DIR = $(SRC_DIR)/Roles
GUI_DIR = $(SRC_DIR)/GUI
OBJ_DIR = obj

# Automatically gather all .cpp source files
SRC_SRCS   = $(wildcard $(SRC_DIR)/*.cpp)
ROLES_SRCS = $(wildcard $(ROLES_DIR)/*.cpp)
GUI_SRCS   = $(wildcard $(GUI_DIR)/*.cpp)

# Combine all source lists
SRCS = $(SRC_SRCS) $(ROLES_SRCS) $(GUI_SRCS)

# Map sources to object files with distinct names
OBJS = \
	$(patsubst $(SRC_DIR)/%.cpp,     $(OBJ_DIR)/%.o,        $(SRC_SRCS)) \
	$(patsubst $(ROLES_DIR)/%.cpp,   $(OBJ_DIR)/Roles_%.o,  $(ROLES_SRCS)) \
	$(patsubst $(GUI_DIR)/%.cpp,     $(OBJ_DIR)/GUI_%.o,    $(GUI_SRCS))

# Final executable name
TARGET = main

# Default target: build the main executable
all: $(TARGET)

# Build and run the demo executable
Main: $(TARGET)
	@echo "Running demo..."
	./$(TARGET)

# Compile the unit tests source into an object file
$(OBJ_DIR)/tests.o: tests.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link test runner with all objects
test_runner: $(OBJS) $(OBJ_DIR)/tests.o
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build and run unit tests
test: test_runner
	@echo "Running unit tests..."
	./test_runner

# Run Valgrind memory leak check on the demo executable
valgrind: $(TARGET)
	@echo "Running valgrind..."
	valgrind --leak-check=full --error-exitcode=1 ./$(TARGET)

# Compile rules for source files in src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile rules for source files in src/Roles/
$(OBJ_DIR)/Roles_%.o: $(ROLES_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile rules for source files in src/GUI/
$(OBJ_DIR)/GUI_%.o: $(GUI_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all object files into the main executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Clean up build artifacts
.PHONY: all Main test valgrind clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET) test_runner
