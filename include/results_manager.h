#ifndef RESULTS_MANAGER_H
#define RESULTS_MANAGER_H

#include "ui_state.h"
#include "types.h"
#include <vector>

// Results management functions
bool showFinalResults(const Matrix& maze, bool pathFound, const std::vector<Coordinates>& path, int openedNodes, UIState& uiState);

#endif // RESULTS_MANAGER_H 