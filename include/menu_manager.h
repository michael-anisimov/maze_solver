#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include "ui_state.h"
#include <string>
#include <vector>

// Menu management functions
bool showMainMenu(UIState& uiState, bool& programRunning);
void showFileSelection(UIState& uiState);
void showAlgorithmSelection(UIState& uiState);
void showSpeedSelection(UIState& uiState);
void drawButton(int y, int x, const std::string& text, bool selected);

#endif // MENU_MANAGER_H 