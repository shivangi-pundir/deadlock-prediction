#ifndef PREDICTOR_H
#define PREDICTOR_H

#include "simulator.h"

// Result Structure

// Holds the result of prediction analysis
typedef struct {
    int is_safe;                        // 1 = safe, 0 = unsafe
    int safe_sequence[MAX_PROCESSES];   // Order processes can finish safely
    int deadlocked_processes[MAX_PROCESSES]; // Processes stuck in deadlock
    int num_deadlocked;                 // How many processes are deadlocked
    int risk_level;                     // 0 = safe, 1 = warning, 2 = danger
} PredictionResult;

// Function Declarations

// Banker's Algorithm — checks if system is in safe state
// Returns 1 if safe, 0 if unsafe
int isSafeState(SystemState *state, PredictionResult *result);

// Predicts if a future resource request will cause deadlock
int predictRequest(SystemState *state, int process_id, 
                   int request[], PredictionResult *result);

// Detects cycles in Resource Allocation Graph
int detectCycle(SystemState *state, PredictionResult *result);

// Calculate risk level of current system state
void calculateRiskLevel(SystemState *state, PredictionResult *result);

// Display full prediction result
void displayPredictionResult(PredictionResult *result, int num_processes);

#endif