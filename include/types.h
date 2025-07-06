#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <utility>

// Maze representation as a 2D integer matrix
using Matrix = std::vector<std::vector<int>>;

// Coordinate pair for maze positions (x, y)
using Coordinates = std::pair<int, int>;

// Direction vectors for 4-directional movement
extern const std::vector<std::pair<int, int>> directions;

struct CoordinatesHash {
    std::size_t operator()(const Coordinates& coords) const {
        return std::hash<int>()(coords.first) ^ std::hash<int>()(coords.second);
    }
};

#endif // TYPES_H 