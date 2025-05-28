# Author: dor.cohen15@msmail.ariel.ac.il
 
# Compiler and flags
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Iinclude -Iinclude/Roles -Iinclude/GUI

# Linker flags for SFML (needed by both builds)
LDFLAGS   := -lsfml-graphics -lsfml-window -lsfml-system

# Directories
SRC_DIR     := src
ROLES_DIR   := $(SRC_DIR)/Roles
GUI_DIR     := $(SRC_DIR)/GUI
OBJ_DIR     := obj
ROLES_OBJ   := $(OBJ_DIR)/Roles
GUI_OBJ     := $(OBJ_DIR)/GUI

#------------------------------------------------------------------------
# Source files
SRC_SRCS    := $(wildcard $(SRC_DIR)/*.cpp)
ROLES_SRCS  := $(wildcard $(ROLES_DIR)/*.cpp)
GUI_SRCS    := $(wildcard $(GUI_DIR)/*.cpp)
TEST_SRC    := tests.cpp

# Object files
SRC_OBJS    := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o,           $(filter-out $(SRC_DIR)/main.cpp, $(SRC_SRCS)))
ROLES_OBJS  := $(patsubst $(ROLES_DIR)/%.cpp,   $(ROLES_OBJ)/%.o,     $(ROLES_SRCS))
GUI_OBJS    := $(patsubst $(GUI_DIR)/%.cpp,     $(GUI_OBJ)/%.o,       $(GUI_SRCS))
MAIN_OBJS   := $(OBJ_DIR)/main.o $(SRC_OBJS) $(ROLES_OBJS) $(GUI_OBJS)
TEST_OBJS   := $(SRC_OBJS) $(ROLES_OBJS) $(GUI_OBJS) $(OBJ_DIR)/tests.o

# Binaries
MAIN_BIN    := main
TEST_BIN    := test_runner

.PHONY: all Main test valgrind clean

#------------------------------------------------------------------------
# Default: build GUI app
all: $(MAIN_BIN)

# Build-and-run GUI app
Main: $(MAIN_BIN)
	@echo "Running main..."
	./$(MAIN_BIN)

# Build GUI app (pulls in SFML)
$(MAIN_BIN): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Build test runner (includes all engine & GUI code + tests)
$(TEST_BIN): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Shortcut to run tests
test: $(TEST_BIN)
	@echo "Running unit tests..."
	./$(TEST_BIN)

# Run Valgrind memory leak check on main executable
valgrind: $(MAIN_BIN)
	@echo "Running valgrind leak-check..."
	valgrind --leak-check=full --error-exitcode=1 ./$(MAIN_BIN)

#------------------------------------------------------------------------
# Compile tests.cpp
$(OBJ_DIR)/tests.o: $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile main.cpp
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile src/*.cpp (except main.cpp)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Roles/*.cpp
$(ROLES_OBJ)/%.o: $(ROLES_DIR)/%.cpp
	@mkdir -p $(ROLES_OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile GUI/*.cpp
$(GUI_OBJ)/%.o: $(GUI_DIR)/%.cpp
	@mkdir -p $(GUI_OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#------------------------------------------------------------------------
# Clean up build artifacts and binaries
clean:
	rm -rf $(OBJ_DIR) $(MAIN_BIN) $(TEST_BIN)
# 'make clean' removes all non-essential build files (objects, executables)
