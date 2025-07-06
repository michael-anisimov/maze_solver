#ifndef UI_STATE_H
#define UI_STATE_H

#include <vector>
#include <string>

/**
 * @file ui_state.h
 * @brief User interface state management
 * 
 * This file defines the UIState structure and related UI management functions
 * for the maze pathfinding visualizer.
 */

/**
 * @brief Central state structure for the user interface
 * 
 * Contains all the state information needed to manage the interactive
 * maze pathfinding visualizer, including algorithm selection, file selection,
 * animation speed, and runtime state.
 */
struct UIState {
    // Runtime state
    bool running;        ///< Whether the algorithm is currently running
    bool paused;         ///< Whether the algorithm is paused
    bool restart_requested; ///< Whether a restart has been requested
    
    // Statistics
    int opened_nodes;    ///< Number of nodes explored by the current algorithm
    
    // Configuration
    int current_algorithm;   ///< Index of the currently selected algorithm
    int current_file;        ///< Index of the currently selected maze file
    double speed;            ///< Animation speed in seconds per step
    int selected_option;     ///< Currently selected menu option
    
    // Available options
    std::vector<std::string> algorithms;    ///< List of available algorithms
    std::vector<std::string> files;         ///< List of available maze files
    std::vector<std::string> speeds;        ///< Human-readable speed options
    std::vector<double> speed_values;       ///< Actual speed values in seconds
};

#endif // UI_STATE_H 