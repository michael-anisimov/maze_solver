#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H

#include "types.h"
#include "ui_state.h"

/**
 * @file ui_functions.h
 * @brief User interface function declarations
 * 
 * This file declares all the UI-related functions for the maze pathfinding visualizer,
 * including drawing, input handling, and menu management.
 */

/**
 * @brief Initialize ncurses color pairs for the application
 * 
 * Sets up color schemes for different UI elements including walls, paths,
 * start/end points, and menu items.
 */
void initColors();

/**
 * @brief Draw the current maze state with visual indicators
 * 
 * Renders the maze matrix with appropriate colors and symbols for walls,
 * paths, start/end points, and explored nodes.
 * 
 * @param maze The maze matrix to display
 * @param state Current UI state for configuration
 */
void drawMaze(const Matrix& maze, const UIState& state);

/**
 * @brief Update the status display with current algorithm information
 * 
 * Shows the current algorithm name, opened nodes count, and other
 * relevant statistics in the status bar.
 * 
 * @param state Current UI state containing statistics
 */
void updateStatus(const UIState& state);

/**
 * @brief Handle user input during algorithm execution
 * 
 * Processes keyboard input for pause/resume, quit, and other controls
 * during algorithm visualization.
 * 
 * @param state UI state to update based on input
 * @return true if the algorithm should continue, false to stop
 */
int handleInput(UIState& state);

/**
 * @brief Handle input during animation delays
 * 
 * Processes user input while waiting between animation frames,
 * allowing for responsive controls during visualization.
 * 
 * @param state UI state to update based on input
 * @param maze The maze matrix to check dimensions against
 * @param delay Current animation delay in seconds
 */
void handleInputDuringDelay(UIState& state, const Matrix& maze, double delay);

/**
 * @brief Draw the information panel with algorithm details
 * 
 * Displays algorithm-specific information, statistics, and controls
 * in a dedicated panel area.
 * 
 * @param state Current UI state containing algorithm information
 */
void drawInfoPanel(const UIState& state);

/**
 * @brief Check if terminal size is sufficient for maze display
 * 
 * Validates that the current terminal window is large enough to properly
 * display the maze and UI elements.
 * 
 * @param maze The maze matrix to check dimensions against
 * @param state UI state to update with size information
 */
void checkTerminalSize(const Matrix& maze, UIState& state);

/**
 * @brief Clean up UI resources and restore terminal state
 * 
 * Properly terminates ncurses and restores the terminal to its
 * original state when the application exits.
 */
void cleanupUI();

/**
 * @brief Display final results after algorithm completion
 * 
 * Shows the final path length, nodes explored, and other statistics
 * after an algorithm has finished executing.
 * 
 * @param state Current UI state with final statistics
 * @param pathLength Length of the found path
 */
void showFinalResult(const UIState& state, size_t pathLength);

/**
 * @brief Draw a bordered menu box
 * 
 * Creates a visual border around menu areas with consistent styling.
 * 
 * @param start_y Top row of the box
 * @param end_y Bottom row of the box
 * @param start_x Left column of the box
 * @param end_x Right column of the box
 */
void drawMenuBox(int start_y, int end_y, int start_x, int end_x);

/**
 * @brief Draw the application header
 * 
 * Displays the title and basic information at the top of the screen.
 */
void drawHeader();

#endif // UI_FUNCTIONS_H 