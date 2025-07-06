#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <string>
#include "types.h"

// Forward declarations
struct UIState;

/**
 * @file algorithms.h
 * @brief Pathfinding algorithm implementations
 * 
 * This file declares the interface for all pathfinding algorithms used in the maze solver.
 * Each algorithm implements the same interface but uses different strategies to find
 * the optimal or near-optimal path from start to end.
 */

/**
 * @brief Breadth-First Search pathfinding algorithm
 * 
 * Explores the maze level by level, guaranteeing the shortest path but potentially
 * exploring many unnecessary nodes.
 * 
 * @param maze The maze matrix where 0=path, 1=wall, -1=start, -2=end
 * @param start Starting coordinates
 * @param end Target coordinates
 * @param path Output vector to store the found path
 * @param openedNodes Counter for nodes explored during search
 * @param delay Animation delay (unused parameter for interface consistency)
 * @param state UI state for visualization control
 * @return true if path found, false otherwise
 */
bool findPathBFS(Matrix& maze, const Coordinates& start, const Coordinates& end,
                 std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

/**
 * @brief Depth-First Search pathfinding algorithm
 * 
 * Explores the maze by following paths as deep as possible before backtracking.
 * May not find the shortest path but typically explores fewer nodes than BFS.
 * 
 * @param maze The maze matrix where 0=path, 1=wall, -1=start, -2=end
 * @param start Starting coordinates
 * @param end Target coordinates
 * @param path Output vector to store the found path
 * @param openedNodes Counter for nodes explored during search
 * @param delay Animation delay (unused parameter for interface consistency)
 * @param state UI state for visualization control
 * @return true if path found, false otherwise
 */
bool findPathDFS(Matrix& maze, const Coordinates& start, const Coordinates& end,
                 std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

/**
 * @brief Random Search pathfinding algorithm
 * 
 * Explores the maze randomly, useful for comparison with systematic algorithms.
 * Not guaranteed to find a path or the optimal path.
 * 
 * @param maze The maze matrix where 0=path, 1=wall, -1=start, -2=end
 * @param start Starting coordinates
 * @param end Target coordinates
 * @param path Output vector to store the found path
 * @param openedNodes Counter for nodes explored during search
 * @param delay Animation delay (unused parameter for interface consistency)
 * @param state UI state for visualization control
 * @return true if path found, false otherwise
 */
bool findPathRandomSearch(Matrix& maze, const Coordinates& start, const Coordinates& end,
                         std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

/**
 * @brief Greedy Search pathfinding algorithm
 * 
 * Uses a heuristic (Manhattan distance) to guide the search toward the goal.
 * May not find the shortest path but often explores fewer nodes than BFS.
 * 
 * @param maze The maze matrix where 0=path, 1=wall, -1=start, -2=end
 * @param start Starting coordinates
 * @param end Target coordinates
 * @param path Output vector to store the found path
 * @param openedNodes Counter for nodes explored during search
 * @param delay Animation delay (unused parameter for interface consistency)
 * @param state UI state for visualization control
 * @return true if path found, false otherwise
 */
bool findPathGreedySearch(Matrix& maze, const Coordinates& start, const Coordinates& end,
                         std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

/**
 * @brief A* pathfinding algorithm
 * 
 * Combines the cost-so-far (g) with a heuristic estimate (h) to find the optimal path.
 * Guarantees the shortest path while typically exploring fewer nodes than BFS.
 * 
 * @param maze The maze matrix where 0=path, 1=wall, -1=start, -2=end
 * @param start Starting coordinates
 * @param end Target coordinates
 * @param path Output vector to store the found path
 * @param openedNodes Counter for nodes explored during search
 * @param delay Animation delay (unused parameter for interface consistency)
 * @param state UI state for visualization control
 * @return true if path found, false otherwise
 */
bool findPathAStar(Matrix& maze, const Coordinates& start, const Coordinates& end,
                   std::vector<Coordinates>& path, int& openedNodes, double delay, UIState& state);

#endif // ALGORITHMS_H 