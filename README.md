# Coup Game Implementation

This is an implementation of the Coup card game in C++, featuring both a game engine and a graphical user interface. The game implements various roles and actions in an object-oriented design pattern, with a focus on clean architecture and extensibility.

## Project Structure

The project is organized into several main components:

### Core Game Engine (`include/` & `src/`)
- `Game.hpp/cpp`: Main game logic and state management
- `Player.hpp/cpp`: Base player class and common functionality

### Player Roles (`include/Roles/` & `src/Roles/`)
- `Baron.hpp/cpp`: Special ability to invest and gain bonus from sanctions
- `General.hpp/cpp`: Can prevent coups against other players
- `Governor.hpp/cpp`: Can override and undo tax actions
- `Judge.hpp/cpp`: Can block bribe attempts
- `Merchant.hpp/cpp`: Gets bonus coins when having 3 or more coins
- `Spy.hpp/cpp`: Can inspect other players' coins and prevent arrests
- Each role inherits from the base `Player` class

### GUI Components (`include/GUI/` & `src/GUI/`)
- `StartScreen`: Initial game screen
- `PlayerSelectionScreen`: Player setup and role assignment
- `GameScreen`: Main game interface
- `ReactionPopup`: Displays action results
- `TargetPopup`: Player targeting interface

## Technical Implementation

### Design Patterns
- **Inheritance**: All roles inherit from base Player class
- **Factory Pattern**: Used in player creation and role assignment
- **Observer Pattern**: For GUI updates and game state changes

### Key Features
1. **Robust Turn Management**
   - Handles player elimination
   - Manages turn order and special actions
   - Supports extra turns from bribe actions

2. **Action Validation**
   - Prevents illegal moves
   - Enforces game rules
   - Handles edge cases

3. **GUI Integration**
   - SFML-based graphical interface
   - Interactive player selection
   - Real-time game state visualization

### Building and Running

The project uses a Makefile build system with the following targets:
- `make all`: Build the main game
- `make test`: Run unit tests
- `make clean`: Clean build artifacts
- `make valgrind`: Run memory leak checks

Dependencies:
- C++17 or later
- SFML library for graphics
- Make build system

## Testing

Comprehensive test suite covering:
- Basic game mechanics
- Role-specific abilities
- Edge cases and error conditions
- Player interactions
- Game state management

The tests are organized into 33 test cases, each focusing on specific aspects of the game mechanics and role interactions.

## Documentation

The codebase is thoroughly documented with:
- Class and method documentation
- Function parameter descriptions
- Implementation notes
- Game rule explanations

Each source file contains detailed comments explaining the purpose and functionality of the code.
