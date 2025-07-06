#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include "ui_state.h"
#include "types.h"
#include <vector>

// Application management functions
void runAlgorithm(UIState& uiState);
bool loadMaze(const std::string& filename, Matrix& maze, Coordinates& start, Coordinates& end);
void initializeApplication(UIState& uiState);

#endif // APP_MANAGER_H 