#include "../include/menu_manager.h"
#include "../include/ui_functions.h"
#include <ncurses.h>
#include <algorithm>

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

void showMainMenu(UIState& uiState, bool& programRunning) {
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
                    // This will be handled by the calling function
                    break;
                case 1:  // Select File
                    showFileSelection(uiState);
                    break;
                case 2:  // Select Algorithm
                    showAlgorithmSelection(uiState);
                    break;
                case 3:  // Adjust Speed
                    showSpeedSelection(uiState);
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

void showFileSelection(UIState& uiState) {
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

void showAlgorithmSelection(UIState& uiState) {
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

void showSpeedSelection(UIState& uiState) {
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