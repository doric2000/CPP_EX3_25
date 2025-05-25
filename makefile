CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinclude/Roles

SRC_DIR = src
ROLES_DIR = src/Roles
OBJ_DIR = obj

# Include all .cpp files from src/ and src/Roles/
SRCS = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(ROLES_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(wildcard $(SRC_DIR)/*.cpp)) \
		$(patsubst $(ROLES_DIR)/%.cpp,$(OBJ_DIR)/Roles_%.o,$(wildcard $(ROLES_DIR)/*.cpp))
TARGET = demo

# Rule for .o files in src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for .o files in src/Roles/
$(OBJ_DIR)/Roles_%.o: $(ROLES_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link all object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)