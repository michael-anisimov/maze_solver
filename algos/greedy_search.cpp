#include "../include/algorithms.h"
#include "../include/ui_functions.h"
#include <queue>
#include <map>
#include <unistd.h>
#include <ncurses.h>
#include <cmath>

struct GreedyComparator {
	Coordinates goal;
	GreedyComparator(const Coordinates& g) : goal(g) {}
	bool operator()(const std::pair<int, Coordinates>& a, const std::pair<int, Coordinates>& b) const {
		return a.first > b.first;
	}
};

int manhattanDistance(const Coordinates& a, const Coordinates& b) {
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

bool findPathGreedySearch(Matrix& maze, const Coordinates& start, const Coordinates& end,
						 std::vector<Coordinates>& path, int& openedNodes, double /* delay */, UIState& state)
{
	GreedyComparator comparator(end);
	std::priority_queue<std::pair<int, Coordinates>, std::vector<std::pair<int, Coordinates>>, GreedyComparator> priorityQueue(comparator);
	std::map<Coordinates, Coordinates> parent;
	int startHeuristic = manhattanDistance(start, end);
	priorityQueue.push(std::make_pair(startHeuristic, start));
	parent[start] = start;
	openedNodes = 1;

	while (!priorityQueue.empty())
	{
		// Handle user input
		int inputResult = handleInput(state);
		if (inputResult == 0) {
			return false; // User quit or stopped
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
			usleep(10000); // Small delay when paused
			continue;
		}

		Coordinates current = priorityQueue.top().second;
		priorityQueue.pop();
		if (current == end)
		{
			Coordinates p = current;
			while (p != start)
			{
				path.push_back(p);
				p = parent[p];
			}
			path.push_back(start);
			std::reverse(path.begin(), path.end());
			
			// Draw final path
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

		if (maze[y][x] == 1 || maze[y][x] == 3) // wall or opened
		{
			continue;
		}
		openedNodes++;
		if (maze[y][x] != -1 && maze[y][x] != -2)
			maze[y][x] = 3;

		for (const auto& direction : directions)
		{
			int dx = direction.first;
			int dy = direction.second;
			int newX = x + dx;
			int newY = y + dy;
			if (newX >= 0 && newX < static_cast<int>(maze[0].size()) && newY >= 0 && newY < static_cast<int>(maze.size()))
			{
				Coordinates neighbor = std::make_pair(newX, newY);
				if (parent.find(neighbor) == parent.end())
				{
					parent[neighbor] = current;
					int heuristic = manhattanDistance(neighbor, end);
					priorityQueue.push(std::make_pair(heuristic, neighbor));
				}
			}
		}

		// Update display
		drawMaze(maze, state);
		state.opened_nodes = openedNodes;
		updateStatus(state);
		refresh();
		handleInputDuringDelay(state, maze, state.speed);
	}
	return false;
} 