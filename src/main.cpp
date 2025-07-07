#include "../include/types.h"
#include "../include/ui_state.h"
#include "../include/ui_functions.h"
#include "../include/menu_manager.h"
#include "../include/app_manager.h"
#include <iostream>
#include <ncurses.h>


// Global variables for application state
UIState uiState;
bool programRunning = true;


int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    
    initColors();
    
    initializeApplication(uiState);
    
    while (programRunning) {
        bool should_run_algorithm = showMainMenu(uiState, programRunning);
        
        // Handle "LETS GO!" selection from main menu
        if (should_run_algorithm && programRunning) {
            runAlgorithm(uiState);
        }
    }
    
    // Cleanup
    endwin();
    return 0;
} 