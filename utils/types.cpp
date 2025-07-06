#include "../include/types.h"

// Direction vectors for 4-directional movement: up, down, left, right
const std::vector<std::pair<int, int>> directions = {
    {-1, 0},   // Left
    {1, 0},    // Right
    {0, -1},   // Up
    {0, 1}     // Down
}; 