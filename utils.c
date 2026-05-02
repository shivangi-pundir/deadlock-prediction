#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

// Clear Screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Print Divider
void printDivider() {
    printf("\n────────────────────────────────────────────────\n");
}

// Validate Input
int validateInput(int value, int min, int max) {
    if (value >= min && value <= max)
        return 1;
    printf("❌ Invalid input! Please enter a value between %d and %d\n",
           min, max);
    return 0;
}

// Pause Screen
void pauseScreen() {
    printf("\n⏸  Press Enter to continue...");
    getchar();
    getchar();
}

// Loading Animation
void printLoading(char *message) {
    int i;
    printf("\n%s ", message);
    for (i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        usleep(300000); // 0.3 second delay
    }
    printf("\n");
}

// Reset Prediction Result
void resetPredictionResult(PredictionResult *result) {
    int i;
    result->is_safe       = 0;
    result->num_deadlocked = 0;
    result->risk_level    = 0;

    for (i = 0; i < MAX_PROCESSES; i++) {
        result->safe_sequence[i]        = -1;
        result->deadlocked_processes[i] = -1;
    }
}

// Copy System State
void copySystemState(SystemState *dest, SystemState *src) {
    int i, j;

    dest->num_processes = src->num_processes;
    dest->num_resources = src->num_resources;

    for (j = 0; j < src->num_resources; j++) {
        dest->available[j] = src->available[j];
        dest->total[j]     = src->total[j];
    }

    for (i = 0; i < src->num_processes; i++) {
        for (j = 0; j < src->num_resources; j++) {
            dest->allocation[i][j] = src->allocation[i][j];
            dest->max_demand[i][j] = src->max_demand[i][j];
            dest->need[i][j]       = src->need[i][j];
        }
    }
}

// Print Error
void printError(char *message) {
    printf("\n❌ ERROR: %s\n", message);
}

// Print Success
void printSuccess(char *message) {
    printf("\n✅ SUCCESS: %s\n", message);
}