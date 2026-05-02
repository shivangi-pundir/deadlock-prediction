#include <stdio.h>
#include "simulator.h"

// Initialize System
void initializeSystem(SystemState *state) {
    int i, j;

    // Get number of processes
    printf("\n╔══════════════════════════════════════╗");
    printf("\n║   DEADLOCK PREDICTION SYSTEM v1.0    ║");
    printf("\n╚══════════════════════════════════════╝\n");

    printf("\nEnter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &state->num_processes);
while(state->num_processes < 1 || state->num_processes > MAX_PROCESSES) {
    printf("❌ Invalid! Enter between 1 and %d: ", MAX_PROCESSES);
    scanf("%d", &state->num_processes);
}
    printf("Enter number of resource types (max %d): ", MAX_RESOURCES);
    scanf("%d", &state->num_resources);

    // Get total resources
    printf("\n--- Total Resources Available ---\n");
    for (j = 0; j < state->num_resources; j++) {
        printf("Total units of Resource R%d: ", j);
        scanf("%d", &state->total[j]);
        state->available[j] = state->total[j]; // start as fully available
    }

    // Get allocation matrix
    printf("\n--- Current Allocation Matrix ---\n");
    printf("(How many units each process is currently holding)\n\n");
    for (i = 0; i < state->num_processes; i++) {
        printf("Process P%d:\n", i);
        for (j = 0; j < state->num_resources; j++) {
            printf("  Allocated units of R%d: ", j);
            scanf("%d", &state->allocation[i][j]);
            state->available[j] -= state->allocation[i][j]; // reduce available
        }
    }

    // Get max demand matrix
    printf("\n--- Maximum Demand Matrix ---\n");
    printf("(Maximum units each process may need in total)\n\n");
    for (i = 0; i < state->num_processes; i++) {
        printf("Process P%d:\n", i);
        for (j = 0; j < state->num_resources; j++) {
            printf("  Max demand of R%d: ", j);
            scanf("%d", &state->max_demand[i][j]);
        }
    }

    // Calculate need matrix
    calculateNeed(state);

    printf("\n✅ System initialized successfully!\n");
}

// Calculate Need Matrix
void calculateNeed(SystemState *state) {
    int i, j;
    for (i = 0; i < state->num_processes; i++) {
        for (j = 0; j < state->num_resources; j++) {
            // Need = Maximum Demand - Current Allocation
            state->need[i][j] = state->max_demand[i][j] - state->allocation[i][j];
        }
    }
}

// Display Full System State
void displayState(SystemState *state) {
    int i, j;

    printf("\n╔══════════════════════════════════════╗");
    printf("\n║         CURRENT SYSTEM STATE         ║");
    printf("\n╚══════════════════════════════════════╝\n");

    // Print header for resources
    printf("\n%-12s", "");
    for (j = 0; j < state->num_resources; j++)
        printf("  R%d ", j);
    printf("\n");

    // Allocation Matrix
    printf("\n📊 Allocation Matrix:\n");
    for (i = 0; i < state->num_processes; i++) {
        printf("  Process P%d  ", i);
        for (j = 0; j < state->num_resources; j++)
            printf("[ %d ]", state->allocation[i][j]);
        printf("\n");
    }

    // Max Demand Matrix
    printf("\n📊 Max Demand Matrix:\n");
    for (i = 0; i < state->num_processes; i++) {
        printf("  Process P%d  ", i);
        for (j = 0; j < state->num_resources; j++)
            printf("[ %d ]", state->max_demand[i][j]);
        printf("\n");
    }

    // Need Matrix
    printf("\n📊 Need Matrix:\n");
    for (i = 0; i < state->num_processes; i++) {
        printf("  Process P%d  ", i);
        for (j = 0; j < state->num_resources; j++)
            printf("[ %d ]", state->need[i][j]);
        printf("\n");
    }

    // Available Resources
    printf("\n📊 Available Resources:\n  ");
    for (j = 0; j < state->num_resources; j++)
        printf("R%d:[%d]  ", j, state->available[j]);
    printf("\n");
}

// Display Need Matrix Only
void displayNeedMatrix(SystemState *state) {
    int i, j;
    printf("\n📊 Need Matrix (Remaining Demand):\n");
    printf("%-12s", "");
    for (j = 0; j < state->num_resources; j++)
        printf("  R%d ", j);
    printf("\n");
    for (i = 0; i < state->num_processes; i++) {
        printf("  Process P%d  ", i);
        for (j = 0; j < state->num_resources; j++)
            printf("[ %d ]", state->need[i][j]);
        printf("\n");
    }
}