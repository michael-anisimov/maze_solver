#include "../include/app_manager.h"
#include "../include/ui_functions.h"
#include "../include/algorithms.h"
#include "../include/maze_loader.h"
#include "../include/results_manager.h"
#include <ncurses.h>
#include <vector>

void initializeApplication(UIState& uiState) {
    // Initialize UI state
    uiState.running = false;
    uiState.paused = false;
    uiState.finished = false;
    uiState.restart_requested = false;
    uiState.user_quit = false;
    uiState.opened_nodes = 0;
    uiState.current_algorithm = 0;
    uiState.current_file = 4;  // Default to maze #5 (index 4)
    uiState.speed = 0.1;
    uiState.selected_option = 0;  // Start with "LETS GO!" selected
    
    // Available algorithms
    uiState.algorithms = {"BFS", "DFS", "Random Search", "Greedy Search", "A*"};
    
    // Available files
    uiState.files.clear();
    for (int i = 1; i <= 14; i++) {
        uiState.files.push_back("dataset/" + std::to_string(i) + ".txt");
    }
    
    // Speed options
    uiState.speeds = {"Extremely Slow", "Very Slow", "Slow", "Normal", "Fast", "Very Fast", "Ultra Fast", "Insane", "Ludicrous"};
    uiState.speed_values = {5.0, 1.0, 0.5, 0.1, 0.05, 0.01, 0.001, 0.0005, 0.0001};
}

bool loadMaze(const std::string& filename, Matrix& maze, Coordinates& start, Coordinates& end) {
    maze = readMaze(filename, start, end);
    return !maze.empty();
}

void runAlgorithm(UIState& uiState) {
    while (true) {
        // Load maze data
        Matrix maze;
        Coordinates start, end;
        
        if (!loadMaze(uiState.files[uiState.current_file], maze, start, end)) {
            clear();
            mvprintw(LINES/2, (COLS-30)/2, "Error loading maze file!");
            mvprintw(LINES/2+1, (COLS-30)/2, "Press any key to continue...");
            refresh();
            getch();
            return;
        }
        
        // Initialize UI for pathfinding
        clear();
        drawHeader();
        
        // Check terminal size for maze
        checkTerminalSize(maze, uiState);
        if (uiState.user_quit) {
            uiState.user_quit = false; // Reset for next time
            return;
        }
        
        // Reset state for new algorithm run
        uiState.running = true;
        uiState.paused = false;
        uiState.finished = false;
        uiState.restart_requested = false;
        uiState.user_quit = false;
        uiState.opened_nodes = 0;
        
        // Draw initial maze and status
        drawMaze(maze, uiState);
        updateStatus(uiState);
        refresh();
        
        // Initialize path vector
        std::vector<Coordinates> path;
        int openedNodes = 0;
        
        // Run the selected algorithm
        bool pathFound = false;
        switch (uiState.current_algorithm) {
            case 0:  // BFS
                pathFound = findPathBFS(maze, start, end, path, openedNodes, uiState.speed, uiState);
                break;
            case 1:  // DFS
                pathFound = findPathDFS(maze, start, end, path, openedNodes, uiState.speed, uiState);
                break;
            case 2:  // Random Search
                pathFound = findPathRandomSearch(maze, start, end, path, openedNodes, uiState.speed, uiState);
                break;
            case 3:  // Greedy Search
                pathFound = findPathGreedySearch(maze, start, end, path, openedNodes, uiState.speed, uiState);
                break;
            case 4:  // A*
                pathFound = findPathAStar(maze, start, end, path, openedNodes, uiState.speed, uiState);
                break;
        }
        
        // Algorithm has finished - set running to false immediately
        uiState.running = false;
        
        // Check if user quit immediately after algorithm
        if (uiState.user_quit) {
            return; // Return to main menu immediately
        }
        
        // Check if restart was requested
        if (uiState.restart_requested) {
            continue; // Restart the algorithm
        }
        
        // Algorithm has finished (only if not quit)
        uiState.finished = true;
        
        // Update status display to show finished state
        updateStatus(uiState);
        refresh();
        
        // Show final results briefly, then return to main menu
        showFinalResults(maze, pathFound, path, openedNodes, uiState);
        
        // If restart was requested after final results, restart
        if (uiState.restart_requested) {
            continue;
        }
        
        // Exit the loop and return to main menu
        break;
    }
} 