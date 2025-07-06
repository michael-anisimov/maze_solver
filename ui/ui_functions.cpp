#include "../include/ui_functions.h"
#include "../include/ui_state.h"
#include <ncurses.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <cstdlib>
#include <string>

/**
 * @file ui_functions.cpp
 * @brief User interface implementation using ncurses
 * 
 * This file contains all the UI-related functions for the maze pathfinding visualizer,
 * including drawing, input handling, and menu management.
 */

/**
 * @brief Initialize ncurses color pairs for the application
 * 
 * Sets up color schemes for different UI elements including walls, paths,
 * start/end points, and menu items. Uses a consistent color scheme throughout
 * the application for better user experience.
 */
void initColors() {
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_WHITE, COLOR_BLUE);      // Header (blue background)
        init_pair(2, COLOR_BLACK, COLOR_CYAN);      // Selected button (cyan background)
        init_pair(3, COLOR_WHITE, COLOR_BLACK);     // Normal button (white text)
        init_pair(4, COLOR_CYAN, COLOR_BLACK);      // Instructions
        init_pair(5, COLOR_GREEN, COLOR_BLACK);     // Success/Path
        init_pair(6, COLOR_RED, COLOR_BLACK);       // Error/End point
        init_pair(7, COLOR_YELLOW, COLOR_BLACK);    // Warning/Explored
        init_pair(8, COLOR_WHITE, COLOR_BLACK);     // Status
        init_pair(9, COLOR_CYAN, COLOR_BLACK);      // Box borders
        init_pair(10, COLOR_BLACK, COLOR_WHITE);    // Info panel background
    }
}

/**
 * @brief Draw the application header
 * 
 * Displays the title and basic information at the top of the screen.
 * The header provides a consistent visual anchor for the application.
 */
void drawHeader() {
    int max_x;
    getmaxyx(stdscr, std::ignore, max_x);
    std::string title = "Maze Pathfinding Visualizer";
    int title_x = (max_x - static_cast<int>(title.length())) / 2;
    
    attron(A_BOLD | COLOR_PAIR(1));
    for (int x = 0; x < max_x; ++x) {
        mvaddch(0, x, ' ');
    }
    mvprintw(0, title_x, "%s", title.c_str());
    attroff(A_BOLD | COLOR_PAIR(1));
}

/**
 * @brief Draw the current maze state with visual indicators
 * 
 * Renders the maze matrix with appropriate colors and symbols for walls,
 * paths, start/end points, and explored nodes. The maze is centered on
 * the screen for optimal viewing.
 * 
 * @param maze The maze matrix to display
 * @param state Current UI state for configuration
 */
void drawMaze(const Matrix& maze, const UIState& /* state */) {
    drawHeader();
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    // Calculate maze display position (centered)
    int maze_height = static_cast<int>(maze.size());
    int maze_width = static_cast<int>(maze[0].size());
    int start_y = (max_y - maze_height) / 2;
    int start_x = (max_x - maze_width) / 2;
    
    // Clear maze area
    for (int y = 0; y < maze_height; y++) {
        for (int x = 0; x < maze_width; x++) {
            mvprintw(start_y + y, start_x + x, " ");
        }
    }
    
    // Draw maze with appropriate symbols and colors
    for (int y = 0; y < maze_height; y++) {
        for (int x = 0; x < maze_width; x++) {
            int cell = maze[y][x];
            char symbol;
            
            switch (cell) {
                case 0:  // Path
                    symbol = ' ';
                    break;
                case 1:  // Wall
                    symbol = '#';
                    attron(COLOR_PAIR(9));
                    break;
                case 2:  // Final path
                    symbol = '*';
                    attron(COLOR_PAIR(5));
                    break;
                case 3:  // Explored
                    symbol = '.';
                    attron(COLOR_PAIR(7));
                    break;
                case -1: // Start
                    symbol = 'S';
                    attron(COLOR_PAIR(5));
                    break;
                case -2: // End
                    symbol = 'E';
                    attron(COLOR_PAIR(6));
                    break;
                default:
                    symbol = '?';
                    break;
            }
            
            mvprintw(start_y + y, start_x + x, "%c", symbol);
            
            // Reset all color attributes
            attroff(COLOR_PAIR(1));
            attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(6));
            attroff(COLOR_PAIR(7));
            attroff(COLOR_PAIR(9));
        }
    }
}

/**
 * @brief Update the status display with current algorithm information
 * 
 * Shows the current algorithm name, opened nodes count, and other
 * relevant statistics in the status bar at the bottom of the screen.
 * 
 * @param state Current UI state containing statistics
 */
void updateStatus(const UIState& state) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    // Clear status area (bottom 5 lines)
    for (int y = max_y - 5; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            mvprintw(y, x, " ");
        }
    }
    
    // Draw status information in organized boxes
    int box_start_y = max_y - 5;
    int num_boxes = 5;
    int box_spacing = 1;
    int total_spacing = (num_boxes + 1) * box_spacing;
    int base_box_width = (max_x - total_spacing) / num_boxes;
    int remainder = (max_x - total_spacing) % num_boxes;
    int x = box_spacing;

    // Algorithm box
    int alg_box_width = base_box_width;
    drawMenuBox(box_start_y, box_start_y + 2, x, x + alg_box_width);
    attron(COLOR_PAIR(3));
    mvprintw(box_start_y + 1, x + 2, "Algorithm: %s", 
             state.algorithms[state.current_algorithm].c_str());
    attroff(COLOR_PAIR(3));
    x += alg_box_width + box_spacing;

    // File box
    int file_box_width = base_box_width;
    drawMenuBox(box_start_y, box_start_y + 2, x, x + file_box_width);
    attron(COLOR_PAIR(3));
    mvprintw(box_start_y + 1, x + 2, "File: %s", 
             state.files[state.current_file].c_str());
    attroff(COLOR_PAIR(3));
    x += file_box_width + box_spacing;

    // Nodes explored box
    int nodes_box_width = base_box_width;
    drawMenuBox(box_start_y, box_start_y + 2, x, x + nodes_box_width);
    attron(COLOR_PAIR(3));
    mvprintw(box_start_y + 1, x + 2, "Explored: %d", state.opened_nodes);
    attroff(COLOR_PAIR(3));
    x += nodes_box_width + box_spacing;

    // Speed box
    int speed_box_width = base_box_width;
    drawMenuBox(box_start_y, box_start_y + 2, x, x + speed_box_width);
    attron(COLOR_PAIR(3));
    mvprintw(box_start_y + 1, x + 2, "Speed: %.3fs", state.speed);
    attroff(COLOR_PAIR(3));
    x += speed_box_width + box_spacing;

    // Status box (last box gets any extra width)
    int status_box_width = base_box_width + remainder;
    drawMenuBox(box_start_y, box_start_y + 2, x, x + status_box_width);
    attron(COLOR_PAIR(3));
    std::string status = state.running ? (state.paused ? "PAUSED" : "RUNNING") : "STOPPED";
    mvprintw(box_start_y + 1, x + 2, "Status: %s", status.c_str());
    attroff(COLOR_PAIR(3));
    
    // Controls at the very bottom
    attron(COLOR_PAIR(4));
    mvprintw(max_y - 1, 2, "Controls: Q=Quit, P=Pause, R=Restart, Up/Down=Speed");
    attroff(COLOR_PAIR(4));
}

/**
 * @brief Handle user input during algorithm execution
 * 
 * Processes keyboard input for pause/resume, quit, and other controls
 * during algorithm visualization. Uses non-blocking input to maintain
 * responsiveness.
 * 
 * @param state UI state to update based on input
 * @return true if the algorithm should continue, false to stop
 */
int handleInput(UIState& state) {
    nodelay(stdscr, TRUE);
    int ch = getch();
    nodelay(stdscr, FALSE);
    
    // Check for terminal resize
    if (ch == KEY_RESIZE) {
        clear();
        refresh();
        return 2;  // Special value for resize
    }
    
    switch (ch) {
        case 'q':
        case 'Q':
            return 0;  // Quit
        case 'r':
        case 'R':
            state.restart_requested = true;
            return 0;  // Stop current algorithm to trigger restart
        case 'p':
        case 'P':
            state.paused = !state.paused;
            state.running = !state.paused;
            updateStatus(state);
            refresh();
            break;
        case KEY_UP:
            // Speed up with fixed steps
            if (state.speed > 0.001) {
                if (state.speed <= 0.001) state.speed = 0.001;
                else if (state.speed <= 0.0025) state.speed = 0.001;
                else if (state.speed <= 0.005) state.speed = 0.0025;
                else if (state.speed <= 0.01) state.speed = 0.005;
                else if (state.speed <= 0.025) state.speed = 0.01;
                else if (state.speed <= 0.05) state.speed = 0.025;
                else if (state.speed <= 0.1) state.speed = 0.05;
                else if (state.speed <= 0.2) state.speed = 0.1;
                else if (state.speed <= 0.5) state.speed = 0.2;
                else state.speed = 0.5;
            }
            updateStatus(state);
            refresh();
            break;
        case KEY_DOWN:
            // Slow down with fixed steps
            if (state.speed < 1.0) {
                if (state.speed >= 1.0) state.speed = 1.0;
                else if (state.speed >= 0.5) state.speed = 1.0;
                else if (state.speed >= 0.2) state.speed = 0.5;
                else if (state.speed >= 0.1) state.speed = 0.2;
                else if (state.speed >= 0.05) state.speed = 0.1;
                else if (state.speed >= 0.025) state.speed = 0.05;
                else if (state.speed >= 0.01) state.speed = 0.025;
                else if (state.speed >= 0.005) state.speed = 0.01;
                else if (state.speed >= 0.0025) state.speed = 0.005;
                else state.speed = 0.0025;
            }
            updateStatus(state);
            refresh();
            break;
    }
    
    return 1;  // Continue
}

/**
 * @brief Handle input during animation delays
 * 
 * Processes user input while waiting between animation frames,
 * allowing for responsive controls during visualization.
 * 
 * @param state UI state to update based on input
 * @param maze The maze matrix to redraw on resize
 * @param delay Current animation delay in seconds
 */
void handleInputDuringDelay(UIState& state, const Matrix& maze, double delay) {
    if (delay <= 0) return;
    
    int delay_ms = static_cast<int>(delay * 1000000);  // Convert to microseconds
    int step_ms = 10000;  // Check input every 10ms
    
    for (int elapsed = 0; elapsed < delay_ms; elapsed += step_ms) {
        int inputResult = handleInput(state);
        if (inputResult == 0) {
            return;  // User requested to stop
        }
        if (inputResult == 2) {
            // Redraw maze and status on resize
            drawMaze(maze, state);
            updateStatus(state);
            refresh();
        }
        usleep(step_ms);
    }
}

/**
 * @brief Draw a bordered menu box
 * 
 * Creates a visual border around menu areas with consistent styling.
 * Uses box-drawing characters for a clean, professional appearance.
 * 
 * @param start_y Top row of the box
 * @param end_y Bottom row of the box
 * @param start_x Left column of the box
 * @param end_x Right column of the box
 */
void drawMenuBox(int start_y, int end_y, int start_x, int end_x) {
    attron(COLOR_PAIR(9));
    
    // Draw corners
    mvaddch(start_y, start_x, ACS_ULCORNER);
    mvaddch(start_y, end_x, ACS_URCORNER);
    mvaddch(end_y, start_x, ACS_LLCORNER);
    mvaddch(end_y, end_x, ACS_LRCORNER);
    
    // Draw horizontal lines
    for (int x = start_x + 1; x < end_x; x++) {
        mvaddch(start_y, x, ACS_HLINE);
        mvaddch(end_y, x, ACS_HLINE);
    }
    
    // Draw vertical lines
    for (int y = start_y + 1; y < end_y; y++) {
        mvaddch(y, start_x, ACS_VLINE);
        mvaddch(y, end_x, ACS_VLINE);
    }
    
    attroff(COLOR_PAIR(9));
}

/**
 * @brief Draw the information panel with algorithm details
 * 
 * Displays algorithm-specific information, statistics, and controls
 * in a dedicated panel area. Currently a placeholder for future
 * algorithm-specific information display.
 * 
 * @param state Current UI state containing algorithm information
 */
void drawInfoPanel(const UIState& /* state */) {
    // Placeholder for future algorithm-specific information
    // Could display algorithm complexity, current heuristic values, etc.
}

/**
 * @brief Check if terminal size is sufficient for maze display
 * 
 * Validates that the current terminal window is large enough to properly
 * display the maze and UI elements. Currently a placeholder for future
 * size validation.
 * 
 * @param maze The maze matrix to check dimensions against
 * @param state UI state to update with size information
 */
void checkTerminalSize(const Matrix& maze, UIState& /* state */) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        int terminal_height = w.ws_row;
        int terminal_width = w.ws_col;
        int maze_height = static_cast<int>(maze.size());
        int maze_width = static_cast<int>(maze[0].size());
        
        // Check if terminal is large enough for maze + UI elements
        if (terminal_height < maze_height + 10 || terminal_width < maze_width + 10) {
            // Terminal too small - could show warning or adjust display
        }
    }
}

/**
 * @brief Clean up UI resources and restore terminal state
 * 
 * Properly terminates ncurses and restores the terminal to its
 * original state when the application exits.
 */
void cleanupUI() {
    endwin();
}

/**
 * @brief Display final results after algorithm completion
 * 
 * Shows the final path length, nodes explored, and other statistics
 * after an algorithm has finished executing.
 * 
 * @param state Current UI state with final statistics
 * @param pathLength Length of the found path
 */
void showFinalResult(const UIState& state, size_t pathLength) {
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);
    
    // Clear bottom area for results
    for (int y = max_y - 3; y < max_y; y++) {
        for (int x = 0; x < max_x; x++) {
            mvprintw(y, x, " ");
        }
    }
    
    // Display results
    attron(COLOR_PAIR(5));
    mvprintw(max_y - 3, 2, "Algorithm completed!");
    mvprintw(max_y - 2, 2, "Path length: %zu nodes", pathLength);
    mvprintw(max_y - 1, 2, "Nodes explored: %d", state.opened_nodes);
    attroff(COLOR_PAIR(5));
} 