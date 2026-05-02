#ifndef UTILS_H
#define UTILS_H

#include "simulator.h"
#include "predictor.h"

// ─── Function Declarations ───────────────────────────────

// Clear terminal screen
void clearScreen();

// Print a divider line
void printDivider();

// Validate user input is within range
int validateInput(int value, int min, int max);

// Pause and wait for user to press enter
void pauseScreen();

// Print loading animation
void printLoading(char *message);

// Reset prediction result to default values
void resetPredictionResult(PredictionResult *result);

// Copy system state (for backup before testing)
void copySystemState(SystemState *dest, SystemState *src);

// Print error message
void printError(char *message);

// Print success message
void printSuccess(char *message);

#endif