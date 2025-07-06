#include "../include/types.h"
#include "../include/algorithms.h"
#include "../include/ui_state.h"
#include "../include/ui_functions.h"
#include "../include/maze_loader.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <utility>
#include <map>

/**
 * @file main.cpp
 * @brief Main application entry point for the Maze Pathfinding Visualizer
 * 
 * This file contains the main application logic, menu system, and program flow
 * for the interactive maze pathfinding visualizer. It manages the user interface,
 * file loading, algorithm selection, and execution coordination.
 */

// Global variables for application state
UIState uiState;
bool programRunning = true;

// Function declarations
bool loadMaze(const std::string& filename, Matrix& maze, Coordinates& start, Coordinates& end);
void showMainMenu();
void showFileSelection();
void showAlgorithmSelection();
void showSpeedSelection();
void runAlgorithm();
bool showFinalResults(const Matrix& maze, bool pathFound, const std::vector<Coordinates>& path, int openedNodes);
void drawButton(int y, int x, const std::string& text, bool selected);

/**
 * @brief Main application entry point
 * 
 * Initializes the ncurses interface, sets up the application state,
 * and runs the main menu loop until the user chooses to quit.
 * 
 * @return 0 on successful execution
 */
int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    // Initialize colors
    initColors();
    
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
    
    // Main application loop
    while (programRunning) {
        showMainMenu();
    }
    
    // Cleanup
    endwin();
    return 0;
}

/**
 * @brief Draw a button with proper styling
 * 
 * Renders a button with appropriate colors and formatting based on
 * whether it's selected or not.
 * 
 * @param y Vertical position
 * @param x Horizontal position
 * @param text Button text
 * @param selected Whether the button is currently selected
 */
void drawButton(int y, int x, const std::string& text, bool selected) {
    if (selected) {
        attron(A_REVERSE | COLOR_PAIR(2));
    } else {
        attron(COLOR_PAIR(3));
    }
    
    // Draw button with better styling
    if (selected) {
        mvprintw(y, x, "> %s <", text.c_str());
    } else {
        mvprintw(y, x, "  %s  ", text.c_str());
    }
    
    if (selected) {
        attroff(A_REVERSE | COLOR_PAIR(2));
    } else {
        attroff(COLOR_PAIR(3));
    }
}

/**
 * @brief Display the main menu
 * 
 * Shows the main application menu with options for file selection,
 * algorithm selection, speed adjustment, and program execution.
 * Handles user navigation and menu selection.
 */
void showMainMenu() {
    clear();
    drawHeader();
    
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    // Menu options
    std::vector<std::string> options = {
        "LETS GO!",
        "Select Maze",
        "Select Algorithm", 
        "Adjust Speed",
        "Quit"
    };
    
    // Calculate menu box dimensions
    int max_option_width = 0;
    for (const std::string& option : options) {
        max_option_width = std::max(max_option_width, static_cast<int>(option.length()));
    }
    int button_width = max_option_width + 6;  // 6 for '> <' or padding
    int box_width = button_width + 6;  // 3 padding each side
    int box_height = 1 + 1 + static_cast<int>(options.size()) * 2 - 1 + 2;  // 1 title, 1 blank, (2*options-1) for buttons+spaces, 2 borders
    
    // Center the menu box
    int box_start_x = (max_x - box_width) / 2;
    int box_start_y = (max_y - box_height) / 2;
    int box_end_x = box_start_x + box_width;
    int box_end_y = box_start_y + box_height;
    
    // Draw menu box
    drawMenuBox(box_start_y, box_end_y, box_start_x, box_end_x);
    
    // Draw menu title
    std::string menu_title = "MAIN MENU";
    int title_x = box_start_x + (box_width - static_cast<int>(menu_title.length())) / 2 + 1;
    attron(A_BOLD | COLOR_PAIR(1));
    mvprintw(box_start_y + 1, title_x, "%s", menu_title.c_str());
    attroff(A_BOLD | COLOR_PAIR(1));
    
    // Draw menu options with blank lines between
    int option_start_y = box_start_y + 3;
    int inner_width = box_width - 1 - 1;  // exclude left and right border
    for (size_t i = 0; i < options.size(); i++) {
        std::string button_str;
        if (static_cast<int>(i) == uiState.selected_option) {
            button_str = "> " + options[i] + " <";
        } else {
            button_str = "  " + options[i] + "  ";
        }
        int available = inner_width - static_cast<int>(button_str.length());
        int x = box_start_x + 2 + (available + 1) / 2 - (available % 2);  // extra space to the left if odd
        drawButton(option_start_y + static_cast<int>(i) * 2, x, options[i], static_cast<int>(i) == uiState.selected_option);
    }
    
    // Draw instructions at the very bottom of the screen
    attron(COLOR_PAIR(4));
    mvprintw(max_y - 1, 0, "Navigation: Up/Down | Select: Enter | Quit: Q");
    attroff(COLOR_PAIR(4));
    
    refresh();
    
    // Handle input
    int ch = getch();
    switch (ch) {
        case KEY_UP:
            uiState.selected_option = (uiState.selected_option - 1 + static_cast<int>(options.size())) % static_cast<int>(options.size());
            break;
        case KEY_DOWN:
            uiState.selected_option = (uiState.selected_option + 1) % static_cast<int>(options.size());
            break;
        case 10:  // Enter
            switch (uiState.selected_option) {
                case 0:  // Start Pathfinding
                    runAlgorithm();
                    break;
                case 1:  // Select File
                    showFileSelection();
                    break;
                case 2:  // Select Algorithm
                    showAlgorithmSelection();
                    break;
                case 3:  // Adjust Speed
                    showSpeedSelection();
                    break;
                case 4:  // Quit
                    programRunning = false;
                    break;
            }
            break;
        case 'q':
        case 'Q':
            programRunning = false;
            break;
    }
}

/**
 * @brief Display file selection menu
 * 
 * Shows a menu allowing the user to select which maze file to use
 * for the pathfinding algorithm.
 */
void showFileSelection() {
    // Store the current selection to restore it when returning
    int original_selection = uiState.selected_option;
    
    while (true) {
        clear();
        drawHeader();
        
        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate box dimensions for file list
        int max_file_width = 0;
        for (const std::string& file : uiState.files) {
            max_file_width = std::max(max_file_width, static_cast<int>(file.length()));
        }
        int button_width = max_file_width + 6;
        int box_width = button_width + 6;
        int box_height = 1 + 1 + static_cast<int>(uiState.files.size()) * 2 - 1 + 2;
        
        // Center the menu box
        int box_start_x = (max_x - box_width) / 2;
        int box_start_y = (max_y - box_height) / 2;
        int box_end_x = box_start_x + box_width;
        int box_end_y = box_start_y + box_height;
        
        // Draw menu box
        drawMenuBox(box_start_y, box_end_y, box_start_x, box_end_x);
        
        // Draw menu title
        std::string menu_title = "SELECT MAZE FILE";
        int title_x = box_start_x + (box_width - static_cast<int>(menu_title.length())) / 2 + 1;
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(box_start_y + 1, title_x, "%s", menu_title.c_str());
        attroff(A_BOLD | COLOR_PAIR(1));
        
        // Draw file options
        int option_start_y = box_start_y + 3;
        int inner_width = box_width - 1 - 1;
        for (size_t i = 0; i < uiState.files.size(); i++) {
            std::string button_str;
            if (static_cast<int>(i) == uiState.selected_option) {
                button_str = "> " + uiState.files[i] + " <";
            } else {
                button_str = "  " + uiState.files[i] + "  ";
            }
            int available = inner_width - static_cast<int>(button_str.length());
            int x = box_start_x + 2 + (available + 1) / 2 - (available % 2);
            drawButton(option_start_y + static_cast<int>(i) * 2, x, uiState.files[i], static_cast<int>(i) == uiState.selected_option);
        }
        
        // Draw instructions
        attron(COLOR_PAIR(4));
        mvprintw(max_y - 1, 0, "Navigation: Up/Down | Select: Enter | Back: Q");
        attroff(COLOR_PAIR(4));
        
        refresh();
        
        // Handle input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                uiState.selected_option = (uiState.selected_option - 1 + static_cast<int>(uiState.files.size())) % static_cast<int>(uiState.files.size());
                break;
            case KEY_DOWN:
                uiState.selected_option = (uiState.selected_option + 1) % static_cast<int>(uiState.files.size());
                break;
            case 10:  // Enter
                uiState.current_file = uiState.selected_option;
                return;
            case 'q':
            case 'Q':
                // Restore original selection if user quits without selecting
                uiState.selected_option = original_selection;
                return;
        }
    }
}

/**
 * @brief Display algorithm selection menu
 * 
 * Shows a menu allowing the user to select which pathfinding algorithm
 * to use for the maze solving.
 */
void showAlgorithmSelection() {
    // Store the current selection to restore it when returning
    int original_selection = uiState.selected_option;
    
    while (true) {
        clear();
        drawHeader();
        
        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate box dimensions for algorithm list
        int max_alg_width = 0;
        for (const std::string& alg : uiState.algorithms) {
            max_alg_width = std::max(max_alg_width, static_cast<int>(alg.length()));
        }
        int button_width = max_alg_width + 6;
        int box_width = button_width + 6;
        int box_height = 1 + 1 + static_cast<int>(uiState.algorithms.size()) * 2 - 1 + 2;
        
        // Center the menu box
        int box_start_x = (max_x - box_width) / 2;
        int box_start_y = (max_y - box_height) / 2;
        int box_end_x = box_start_x + box_width;
        int box_end_y = box_start_y + box_height;
        
        // Draw menu box
        drawMenuBox(box_start_y, box_end_y, box_start_x, box_end_x);
        
        // Draw menu title
        std::string menu_title = "SELECT ALGORITHM";
        int title_x = box_start_x + (box_width - static_cast<int>(menu_title.length())) / 2 + 1;
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(box_start_y + 1, title_x, "%s", menu_title.c_str());
        attroff(A_BOLD | COLOR_PAIR(1));
        
        // Draw algorithm options
        int option_start_y = box_start_y + 3;
        int inner_width = box_width - 1 - 1;
        for (size_t i = 0; i < uiState.algorithms.size(); i++) {
            std::string button_str;
            if (static_cast<int>(i) == uiState.selected_option) {
                button_str = "> " + uiState.algorithms[i] + " <";
            } else {
                button_str = "  " + uiState.algorithms[i] + "  ";
            }
            int available = inner_width - static_cast<int>(button_str.length());
            int x = box_start_x + 2 + (available + 1) / 2 - (available % 2);
            drawButton(option_start_y + static_cast<int>(i) * 2, x, uiState.algorithms[i], static_cast<int>(i) == uiState.selected_option);
        }
        
        // Draw instructions
        attron(COLOR_PAIR(4));
        mvprintw(max_y - 1, 0, "Navigation: Up/Down | Select: Enter | Back: Q");
        attroff(COLOR_PAIR(4));
        
        refresh();
        
        // Handle input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                uiState.selected_option = (uiState.selected_option - 1 + static_cast<int>(uiState.algorithms.size())) % static_cast<int>(uiState.algorithms.size());
                break;
            case KEY_DOWN:
                uiState.selected_option = (uiState.selected_option + 1) % static_cast<int>(uiState.algorithms.size());
                break;
            case 10:  // Enter
                uiState.current_algorithm = uiState.selected_option;
                return;
            case 'q':
            case 'Q':
                // Restore original selection if user quits without selecting
                uiState.selected_option = original_selection;
                return;
        }
    }
}

/**
 * @brief Display speed selection menu
 * 
 * Shows a menu allowing the user to select the animation speed
 * for the pathfinding visualization.
 */
void showSpeedSelection() {
    // Store the current selection to restore it when returning
    int original_selection = uiState.selected_option;
    
    while (true) {
        clear();
        drawHeader();
        
        int max_x, max_y;
        getmaxyx(stdscr, max_y, max_x);
        
        // Calculate box dimensions for speed list
        int max_speed_width = 0;
        for (const std::string& speed : uiState.speeds) {
            max_speed_width = std::max(max_speed_width, static_cast<int>(speed.length()));
        }
        int button_width = max_speed_width + 6;
        int box_width = button_width + 6;
        int box_height = 1 + 1 + static_cast<int>(uiState.speeds.size()) * 2 - 1 + 2;
        
        // Center the menu box
        int box_start_x = (max_x - box_width) / 2;
        int box_start_y = (max_y - box_height) / 2;
        int box_end_x = box_start_x + box_width;
        int box_end_y = box_start_y + box_height;
        
        // Draw menu box
        drawMenuBox(box_start_y, box_end_y, box_start_x, box_end_x);
        
        // Draw menu title
        std::string menu_title = "SELECT SPEED";
        int title_x = box_start_x + (box_width - static_cast<int>(menu_title.length())) / 2 + 1;
        attron(A_BOLD | COLOR_PAIR(1));
        mvprintw(box_start_y + 1, title_x, "%s", menu_title.c_str());
        attroff(A_BOLD | COLOR_PAIR(1));
        
        // Draw speed options
        int option_start_y = box_start_y + 3;
        int inner_width = box_width - 1 - 1;
        for (size_t i = 0; i < uiState.speeds.size(); i++) {
            std::string button_str;
            if (static_cast<int>(i) == uiState.selected_option) {
                button_str = "> " + uiState.speeds[i] + " <";
            } else {
                button_str = "  " + uiState.speeds[i] + "  ";
            }
            int available = inner_width - static_cast<int>(button_str.length());
            int x = box_start_x + 2 + (available + 1) / 2 - (available % 2);
            drawButton(option_start_y + static_cast<int>(i) * 2, x, uiState.speeds[i], static_cast<int>(i) == uiState.selected_option);
        }
        
        // Draw instructions
        attron(COLOR_PAIR(4));
        mvprintw(max_y - 1, 0, "Navigation: Up/Down | Select: Enter | Back: Q");
        attroff(COLOR_PAIR(4));
        
        refresh();
        
        // Handle input
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                uiState.selected_option = (uiState.selected_option - 1 + static_cast<int>(uiState.speeds.size())) % static_cast<int>(uiState.speeds.size());
                break;
            case KEY_DOWN:
                uiState.selected_option = (uiState.selected_option + 1) % static_cast<int>(uiState.speeds.size());
                break;
            case 10:  // Enter
                uiState.speed = uiState.speed_values[uiState.selected_option];
                return;
            case 'q':
            case 'Q':
                // Restore original selection if user quits without selecting
                uiState.selected_option = original_selection;
                return;
        }
    }
}

/**
 * @brief Execute the selected pathfinding algorithm
 * 
 * Loads the selected maze file and runs the chosen algorithm with
 * the current settings. Handles the visualization and result display.
 */
void runAlgorithm() {
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
        showFinalResults(maze, pathFound, path, openedNodes);
        
        // If restart was requested after final results, restart
        if (uiState.restart_requested) {
            continue;
        }
        
        // Exit the loop and return to main menu
        break;
    }
}

/**
 * @brief Display final results after algorithm completion
 * 
 * Shows the results of the pathfinding algorithm including whether
 * a path was found, the path length, and nodes explored.
 * 
 * @param maze The maze matrix
 * @param pathFound Whether a path was successfully found
 * @param path The found path (if any)
 * @param openedNodes Number of nodes explored during the search
 */
bool showFinalResults(const Matrix& maze, bool pathFound, const std::vector<Coordinates>& path, int openedNodes) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    auto drawFinalInfo = [&]() {
        getmaxyx(stdscr, max_y, max_x);
        int result_y = 2;
        for (int y = result_y; y < result_y + 3; y++) {
            for (int x = 0; x < max_x; x++) {
                mvprintw(y, x, " ");
            }
        }
        int num_boxes = 2;
        int box_spacing = 1;
        int total_spacing = (num_boxes + 1) * box_spacing;
        int base_box_width = (max_x - total_spacing) / num_boxes;
        int remainder = (max_x - total_spacing) % num_boxes;
        int x = box_spacing;
        
        // Completion status box
        int status_box_width = base_box_width;
        drawMenuBox(result_y, result_y + 2, x, x + status_box_width);
        attron(COLOR_PAIR(5));
        if (pathFound) {
            mvprintw(result_y + 1, x + 2, "Completed!");
        } else {
            mvprintw(result_y + 1, x + 2, "No path!");
        }
        attroff(COLOR_PAIR(5));
        x += status_box_width + box_spacing;
        
        // Path length box (only if path found)
        int path_box_width = base_box_width + remainder;
        drawMenuBox(result_y, result_y + 2, x, x + path_box_width);
        attron(COLOR_PAIR(5));
        if (pathFound) {
            mvprintw(result_y + 1, x + 2, "Length: %zu nodes", path.size());
        } else {
            mvprintw(result_y + 1, x + 2, "Explored: %d", openedNodes);
        }
        attroff(COLOR_PAIR(5));
        
        attron(COLOR_PAIR(4));
        mvprintw(max_y - 1, 2, "Controls: R=Restart, Q=Menu, Up/Down=Speed");
        attroff(COLOR_PAIR(4));
        refresh();
    };
    drawFinalInfo();
    
    // Stay on results screen until user explicitly chooses to leave
    while (true) {
        int ch = getch();
        if (ch == 'r' || ch == 'R') {
            uiState.restart_requested = true;
            return true;
        } else if (ch == 'q' || ch == 'Q') {
            return false;
        } else if (ch == KEY_RESIZE) {
            clear();
            drawHeader();
            drawFinalInfo();
            drawMaze(maze, uiState);
            updateStatus(uiState);
            refresh();
        } else if (ch == KEY_UP) {
            // Speed up with fixed steps
            if (uiState.speed > 0.001) {
                if (uiState.speed <= 0.001) uiState.speed = 0.001;
                else if (uiState.speed <= 0.0025) uiState.speed = 0.001;
                else if (uiState.speed <= 0.005) uiState.speed = 0.0025;
                else if (uiState.speed <= 0.01) uiState.speed = 0.005;
                else if (uiState.speed <= 0.025) uiState.speed = 0.01;
                else if (uiState.speed <= 0.05) uiState.speed = 0.025;
                else if (uiState.speed <= 0.1) uiState.speed = 0.05;
                else if (uiState.speed <= 0.2) uiState.speed = 0.1;
                else if (uiState.speed <= 0.5) uiState.speed = 0.2;
                else uiState.speed = 0.5;
            }
            updateStatus(uiState);
            refresh();
        } else if (ch == KEY_DOWN) {
            // Slow down with fixed steps
            if (uiState.speed < 1.0) {
                if (uiState.speed >= 1.0) uiState.speed = 1.0;
                else if (uiState.speed >= 0.5) uiState.speed = 0.5;
                else if (uiState.speed >= 0.2) uiState.speed = 0.5;
                else if (uiState.speed >= 0.1) uiState.speed = 0.2;
                else if (uiState.speed >= 0.05) uiState.speed = 0.1;
                else if (uiState.speed >= 0.025) uiState.speed = 0.05;
                else if (uiState.speed >= 0.01) uiState.speed = 0.025;
                else if (uiState.speed >= 0.005) uiState.speed = 0.01;
                else if (uiState.speed >= 0.0025) uiState.speed = 0.005;
                else uiState.speed = 0.0025;
            }
            updateStatus(uiState);
            refresh();
        }
        // For any other key, stay on the results screen
    }
}

/**
 * @brief Load maze data from file
 * 
 * Loads maze data from the specified file and extracts start/end positions.
 * This is a wrapper around the readMaze function for consistency.
 * 
 * @param filename Path to the maze file
 * @param maze Output parameter for the maze matrix
 * @param start Output parameter for start coordinates
 * @param end Output parameter for end coordinates
 * @return true if loading was successful, false otherwise
 */
bool loadMaze(const std::string& filename, Matrix& maze, Coordinates& start, Coordinates& end) {
    maze = readMaze(filename, start, end);
    return !maze.empty();
} 