#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <string>
#include "types.h"

// Forward declarations
struct UIState;

bool findPathBFS(Matrix& maze, const Coordinates& start, const Coordinates& end,
                 std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

bool findPathDFS(Matrix& maze, const Coordinates& start, const Coordinates& end,
                 std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

bool findPathRandomSearch(Matrix& maze, const Coordinates& start, const Coordinates& end,
                         std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

bool findPathGreedySearch(Matrix& maze, const Coordinates& start, const Coordinates& end,
                         std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

bool findPathAStar(Matrix& maze, const Coordinates& start, const Coordinates& end,
                   std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

#endif // ALGORITHMS_H 