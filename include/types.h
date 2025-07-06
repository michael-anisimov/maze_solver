#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <utility>

/**
 * @file types.h
 * @brief Common type definitions for the maze pathfinding project
 * 
 * This file contains the fundamental data types used throughout the application,
 * including maze representation and coordinate systems.
 */

// Maze representation as a 2D integer matrix
using Matrix = std::vector<std::vector<int>>;

// Coordinate pair for maze positions (x, y)
using Coordinates = std::pair<int, int>;

// Direction vectors for 4-directional movement
extern const std::vector<std::pair<int, int>> directions;

/**
 * @brief Hash function for Coordinates to enable use in unordered containers
 * 
 * This struct provides a hash function for the Coordinates type,
 * allowing it to be used as a key in std::unordered_map and similar containers.
 */
struct CoordinatesHash {
    std::size_t operator()(const Coordinates& coords) const {
        return std::hash<int>()(coords.first) ^ std::hash<int>()(coords.second);
    }
};

#endif // TYPES_H 