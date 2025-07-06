#include "../include/algorithms.h"
#include "../include/ui_functions.h"
#include <vector>
#include <map>
	#include <queue>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <ncurses.h>
#include <ctime>

bool findPathRandomSearch(Matrix& maze, const Coordinates& start, const Coordinates& end,
                         std::vector<Coordinates>& path, int& openedNodes, double /* delay */, UIState& state) {
	std::map<Coordinates, Coordinates> parent;
	std::queue<Coordinates> q;
	std::random_device rd;
	std::mt19937 gen(rd());
	q.push(start);
	parent[start] = start;
	openedNodes = 1;
	
	while (!q.empty()) {
		int inputResult = handleInput(state);
		if (inputResult == 0) {
			return false;
		}
		if (inputResult == 2) {
			drawMaze(maze, state);
			updateStatus(state);
			refresh();
		}
		
		// Check for restart request
		if (state.restart_requested) {
			return false; // Stop current algorithm to trigger restart
		}
		
		if (!state.running) {
			usleep(10000);
			continue;
		}
		
		Coordinates current = q.front();
		q.pop();
		
		if (current == end) {
			Coordinates p = current;
			while (p != start) {
				path.push_back(p);
				p = parent[p];
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			for (const auto& p : path) {
				maze[p.second][p.first] = 2;
			}
			maze[start.second][start.first] = -1;
			maze[end.second][end.first] = -2;
			drawMaze(maze, state);
			updateStatus(state);
			refresh();
			return true;
		}
		
		int x = current.first;
		int y = current.second;
		
		if (maze[y][x] == 1 || maze[y][x] == 3) {
			continue;
		}
		
		openedNodes++;
		if (maze[y][x] != -1 && maze[y][x] != -2) {
			maze[y][x] = 3;
		}
		
		std::vector<Coordinates> unvisitedNeighbors;
		for (const auto& direction : directions) {
			int dx = direction.first;
			int dy = direction.second;
			int newX = x + dx;
			int newY = y + dy;
			if (newX >= 0 && newX < static_cast<int>(maze[0].size()) && 
				newY >= 0 && newY < static_cast<int>(maze.size())) {
				Coordinates neighbor = std::make_pair(newX, newY);
				if (parent.find(neighbor) == parent.end()) {
					unvisitedNeighbors.push_back(neighbor);
				}
			}
		}
		
		// Randomly shuffle and add all unvisited neighbors
		if (!unvisitedNeighbors.empty()) {
			std::shuffle(unvisitedNeighbors.begin(), unvisitedNeighbors.end(), gen);
			for (const auto& neighbor : unvisitedNeighbors) {
				parent[neighbor] = current;
				q.push(neighbor);
			}
		}
		
		drawMaze(maze, state);
		state.opened_nodes = openedNodes;
		updateStatus(state);
		refresh();
		handleInputDuringDelay(state, maze, state.speed);
	}
	return false;
} 