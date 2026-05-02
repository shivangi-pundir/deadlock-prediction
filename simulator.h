#ifndef SIMULATOR_H
#define SIMULATOR_H

// ─── Constants ───────────────────────────────────────────
#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// ─── Data Structures ─────────────────────────────────────

// Holds all system state
typedef struct {
    int num_processes;                          // Total number of processes
    int num_resources;                          // Total number of resource types

    int allocation[MAX_PROCESSES][MAX_RESOURCES]; // What each process currently holds
    int max_demand[MAX_PROCESSES][MAX_RESOURCES]; // Maximum demand of each process
    int available[MAX_RESOURCES];               // Currently available resources
    int total[MAX_RESOURCES];                   // Total resources in system

    // Calculated internally
    int need[MAX_PROCESSES][MAX_RESOURCES];     // Remaining need = max - allocation
    int finished[MAX_PROCESSES];                // Track if process completed
} SystemState;

// ─── Function Declarations ───────────────────────────────

// Initialize the system with user input
void initializeSystem(SystemState *state);

// Calculate need matrix (max - allocation)
void calculateNeed(SystemState *state);

// Display all matrices nicely
void displayState(SystemState *state);

// Display need matrix
void displayNeedMatrix(SystemState *state);

#endif