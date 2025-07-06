#include "../include/algorithms.h"
#include "../include/ui_functions.h"
#include <queue>
#include <map>
#include <unistd.h>
#include <ncurses.h>

bool findPathBFS(Matrix &maze, const Coordinates &start, const Coordinates &end, 
                    std::vector<Coordinates> &path, int &openedNodes, double /* delay */, UIState& state)
{
	std::queue<Coordinates> q;
	std::map<Coordinates, Coordinates> parent;
	q.push(start);
	parent[start] = start;
	openedNodes++;

	while (!q.empty())
	{
		// Check for immediate quit first
		if (state.user_quit) {
			return false;
		}
		
		// Handle user input
		int inputResult = handleInput(state);
		if (inputResult == 0) {
			return false; // User quit or stopped
		}
		if (inputResult == 4) {
			return false; // Immediate quit
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

		Coordinates current = q.front();
		q.pop();
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
			for (auto p : path) {
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

		for (auto d : directions)
		{
			int dx = d.first;
			int dy = d.second;
			if (x + dx >= 0 && x + dx < (int)maze[0].size() && y + dy >= 0 && y + dy < (int)maze.size())
			{
				Coordinates next = std::make_pair(x + dx, y + dy);
				if (parent.find(next) == parent.end())
				{
					parent[next] = current;
					q.push(next);
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