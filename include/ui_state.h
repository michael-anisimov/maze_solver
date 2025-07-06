#ifndef UI_STATE_H
#define UI_STATE_H

#include <vector>
#include <string>

struct UIState {
    // Runtime state
    bool running;        ///< Whether the algorithm is currently running
    bool paused;         ///< Whether the algorithm is paused
    bool finished;       ///< Whether the algorithm has finished
    bool restart_requested; ///< Whether a restart has been requested
    bool user_quit;      ///< Whether the user quit the algorithm
    
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