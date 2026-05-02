#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "simulator.h"
#include "predictor.h"

// ─── Function Declarations ───────────────────────────────

// Print welcome banner
void displayBanner();

// Display main menu
void displayMenu();

// Display Resource Allocation Graph as text
void displayRAG(SystemState *state);

// Display system summary in a clean table
void displaySummaryTable(SystemState *state, PredictionResult *result);

// Display process status (waiting/running/finished)
void displayProcessStatus(SystemState *state, PredictionResult *result);

// Display resource status (free/allocated)
void displayResourceStatus(SystemState *state);

// Save results to output file
void saveResultsToFile(SystemState *state, PredictionResult *result);

#endif