CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinclude/Roles -Iinclude/GUI
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC_DIR = src
ROLES_DIR = $(SRC_DIR)/Roles
GUI_DIR = $(SRC_DIR)/GUI
OBJ_DIR = obj

# Automatically gather source files
SRC_SRCS   = $(wildcard $(SRC_DIR)/*.cpp)
ROLES_SRCS = $(wildcard $(ROLES_DIR)/*.cpp)
GUI_SRCS   = $(wildcard $(GUI_DIR)/*.cpp)

# Combine all sources
SRCS = $(SRC_SRCS) $(ROLES_SRCS) $(GUI_SRCS)

# Generate flat object filenames
OBJS = \
	$(patsubst $(SRC_DIR)/%.cpp,     $(OBJ_DIR)/%.o,        $(SRC_SRCS)) \
	$(patsubst $(ROLES_DIR)/%.cpp,   $(OBJ_DIR)/Roles_%.o,  $(ROLES_SRCS)) \
	$(patsubst $(GUI_DIR)/%.cpp,     $(OBJ_DIR)/GUI_%.o,    $(GUI_SRCS))

TARGET = main

# Default target
all: $(TARGET)

# Compile rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/Roles_%.o: $(ROLES_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/GUI_%.o: $(GUI_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Clean
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
