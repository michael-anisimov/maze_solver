#include "../include/types.h"

/**
 * @file types.cpp
 * @brief Implementation of common types and constants
 * 
 * This file provides the implementation of the directions vector and other
 * shared constants used throughout the maze pathfinding application.
 */

// Direction vectors for 4-directional movement: up, down, left, right
const std::vector<std::pair<int, int>> directions = {
    {-1, 0},   // Left
    {1, 0},    // Right
    {0, -1},   // Up
    {0, 1}     // Down
}; 