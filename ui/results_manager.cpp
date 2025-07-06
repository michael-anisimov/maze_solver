#include "../include/results_manager.h"
#include "../include/ui_functions.h"
#include <ncurses.h>

bool showFinalResults(const Matrix& maze, bool pathFound, const std::vector<Coordinates>& path, int openedNodes, UIState& uiState) {
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