# Maze Pathfinding Visualizer

An interactive terminal-based application that visualizes various pathfinding algorithms solving mazes in real-time. Built with C++ and ncurses for a rich terminal user interface.

## Features

- **Multiple Algorithms**: BFS, DFS, A*, Greedy Search, and Random Search
- **Real-time Visualization**: Watch algorithms explore the maze step by step
- **Interactive Controls**: Pause, resume, adjust speed, and navigate menus
- **Multiple Mazes**: 14 different maze configurations included
- **Clean Architecture**: Well-organized, documented codebase

## Project Structure

```
task1/
├── include/           # Header files
│   ├── types.h       # Common type definitions
│   ├── algorithms.h  # Algorithm interface declarations
│   ├── ui_state.h    # UI state management
│   └── ui_functions.h # UI function declarations
├── src/              # Main application
│   └── main.cpp      # Application entry point and menu system
├── algorithms/       # Pathfinding algorithm implementations
│   ├── bfs.cpp       # Breadth-First Search
│   ├── dfs.cpp       # Depth-First Search
│   ├── astar.cpp     # A* algorithm
│   ├── greedy_search.cpp # Greedy Search
│   └── random_search.cpp # Random Search
├── ui/               # User interface
│   └── ui_functions.cpp # ncurses UI implementation
├── utils/            # Utility functions
│   ├── types.cpp     # Type implementations
│   └── maze_loader.cpp # Maze file loading utilities
├── build/            # Build artifacts (generated)
├── dataset/          # Maze files
│   ├── 1.txt        # Maze configurations
│   ├── 2.txt
│   └── ...
├── Makefile          # Build configuration
└── README.md         # This file
```

## Algorithms

### Breadth-First Search (BFS)
- **Guarantee**: Finds shortest path
- **Strategy**: Explores level by level
- **Use Case**: When optimal path is required
- **Performance**: May explore many unnecessary nodes

### Depth-First Search (DFS)
- **Guarantee**: Finds a path (not necessarily shortest)
- **Strategy**: Explores deep paths before backtracking
- **Use Case**: When any path is acceptable
- **Performance**: Typically explores fewer nodes than BFS

### A* Algorithm
- **Guarantee**: Finds shortest path
- **Strategy**: Uses heuristic (Manhattan distance) + cost-so-far
- **Use Case**: Optimal path with good performance
- **Performance**: Usually explores fewer nodes than BFS

### Greedy Search
- **Guarantee**: Finds a path (not necessarily shortest)
- **Strategy**: Always chooses neighbor closest to goal
- **Use Case**: Quick path finding
- **Performance**: Often explores fewer nodes than BFS

### Random Search
- **Guarantee**: May find a path (not guaranteed)
- **Strategy**: Randomly selects next neighbor
- **Use Case**: Comparison baseline
- **Performance**: Unpredictable, demonstrates importance of strategy

## Building and Running

### Prerequisites

- C++11 compatible compiler (GCC, Clang)
- ncurses library
- Make

### Installation

#### macOS
```bash
# Install dependencies
brew install ncurses

# Build the project
make

# Run the application
make run
```

#### Linux
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libncurses5-dev

# Build the project
make

# Run the application
make run
```

### Build Targets

- `make` or `make all` - Build the application
- `make clean` - Remove build files
- `make run` - Build and run the application
- `make debug` - Build with debug symbols
- `make release` - Build with optimizations
- `make docs` - Show project documentation
- `make help` - Show available targets

## Usage

### Main Menu
1. **Select File** - Choose which maze to solve
2. **Select Algorithm** - Choose pathfinding algorithm
3. **Adjust Speed** - Set animation speed
4. **Let's go!** - Start the algorithm
5. **Quit** - Exit the application

### During Algorithm Execution
- **Q** - Quit the algorithm
- **P** - Pause/Resume
- **S** - Stop and return to menu
- **↑/↓** - Adjust animation speed

### Navigation
- **↑/↓** - Navigate menu options
- **Enter** - Select option
- **Q** - Go back/Quit

## Maze Format

Maze files are text files with the following format:
```
<width> <height>
<cell_data>
```

Where:
- `width` and `height` are integers
- `cell_data` is a grid of integers where:
  - `0` = Path
  - `1` = Wall
  - `-1` = Start position
  - `-2` = End position

## Code Quality

### Documentation
- Comprehensive Doxygen-style comments
- Clear function and class documentation
- Algorithm explanations with complexity analysis

### Architecture
- **Separation of Concerns**: UI, algorithms, and utilities are separate
- **Clean Interfaces**: Well-defined function signatures
- **Type Safety**: Strong typing with custom types
- **Error Handling**: Graceful error handling throughout

### Code Style
- Consistent naming conventions
- Proper indentation and formatting
- Meaningful variable and function names
- No magic numbers or hardcoded values

## Contributing

1. Follow the existing code style and documentation patterns
2. Add appropriate comments for new functions
3. Update the Makefile if adding new source files
4. Test with multiple maze configurations

## License

This project is part of an educational assignment and is provided as-is for learning purposes.

## Acknowledgments

- ncurses library for terminal UI capabilities
- Educational algorithms and data structures resources
- Terminal-based application design patterns 