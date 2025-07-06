#ifndef UI_FUNCTIONS_H
#define UI_FUNCTIONS_H

#include "types.h"
#include "ui_state.h"

void initColors();
void drawMaze(const Matrix& maze, const UIState& state);
void updateStatus(const UIState& state);
int handleInput(UIState& state);
void handleInputDuringDelay(UIState& state, const Matrix& maze, double delay);
void drawInfoPanel(const UIState& state);
void checkTerminalSize(const Matrix& maze, UIState& state);
void cleanupUI();
void showFinalResult(const UIState& state, size_t pathLength);
void drawMenuBox(int start_y, int end_y, int start_x, int end_x);
void drawHeader();

#endif // UI_FUNCTIONS_H 