#include <stdio.h>
#include <string.h>
#include "predictor.h"

// Banker's Algorithm
int isSafeState(SystemState *state, PredictionResult *result) {
    int i, j, k;
    int work[MAX_RESOURCES];
    int finish[MAX_PROCESSES];
    int safe_index = 0;
    int found;

    // Step 1: Initialize work = available, finish = false
    for (j = 0; j < state->num_resources; j++)
        work[j] = state->available[j];

    for (i = 0; i < state->num_processes; i++)
        finish[i] = 0;

    // Initialize result
    result->num_deadlocked = 0;
    result->is_safe = 0;

    // Step 2: Find a process that can finish
    int count = 0;
    while (count < state->num_processes) {
        found = 0;
        for (i = 0; i < state->num_processes; i++) {
            if (finish[i] == 0) {
                // Check if need <= work
                int can_finish = 1;
                for (j = 0; j < state->num_resources; j++) {
                    if (state->need[i][j] > work[j]) {
                        can_finish = 0;
                        break;
                    }
                }

                // If process can finish
                if (can_finish) {
                    // Release its resources back to work
                    for (k = 0; k < state->num_resources; k++)
                        work[k] += state->allocation[i][k];

                    finish[i] = 1;
                    result->safe_sequence[safe_index++] = i;
                    found = 1;
                    count++;
                }
            }
        }
        // If no process found in this pass — deadlock!
        if (!found) break;
    }

    // Step 3: Check if all processes finished
    if (safe_index == state->num_processes) {
        result->is_safe = 1;
    } else {
        // Find which processes are deadlocked
        result->is_safe = 0;
        for (i = 0; i < state->num_processes; i++) {
            if (finish[i] == 0) {
                result->deadlocked_processes[result->num_deadlocked++] = i;
            }
        }
    }

    // Calculate risk level
    calculateRiskLevel(state, result);

    return result->is_safe;
}

// Predict Future Request
int predictRequest(SystemState *state, int process_id,
                   int request[], PredictionResult *result) {
    int j;

    printf("\n🔍 Analyzing request from Process P%d...\n", process_id);

    // Step 1: Check request <= need
    for (j = 0; j < state->num_resources; j++) {
        if (request[j] > state->need[process_id][j]) {
            printf("❌ ERROR: Process P%d requested more than its maximum need!\n",
                   process_id);
            return -1;
        }
    }

    // Step 2: Check request <= available
    for (j = 0; j < state->num_resources; j++) {
        if (request[j] > state->available[j]) {
            printf("⚠️  Process P%d must WAIT — resources not available!\n",
                   process_id);
            return 0;
        }
    }

    // Step 3: Pretend to allocate and check safety
    // Temporarily modify state
    for (j = 0; j < state->num_resources; j++) {
        state->available[j]              -= request[j];
        state->allocation[process_id][j] += request[j];
        state->need[process_id][j]       -= request[j];
    }

    // Check if still safe
    int safe = isSafeState(state, result);

    // Step 4: Rollback the temporary changes
    for (j = 0; j < state->num_resources; j++) {
        state->available[j]              += request[j];
        state->allocation[process_id][j] -= request[j];
        state->need[process_id][j]       += request[j];
    }

    if (safe) {
        printf("✅ Request is SAFE to grant — no deadlock predicted!\n");
    } else {
        printf("🚨 WARNING: Granting this request WILL cause deadlock!\n");
        printf("   Request DENIED to prevent deadlock.\n");
    }

    return safe;
}

// Cycle Detection (RAG)
int detectCycle(SystemState *state, PredictionResult *result) {
    int i, j;
    // Wait-for graph: visited and recursion stack
    int visited[MAX_PROCESSES];
    int rec_stack[MAX_PROCESSES];
    int wait_for[MAX_PROCESSES][MAX_PROCESSES];

    // Build wait-for graph from need and allocation
    memset(wait_for, 0, sizeof(wait_for));
    memset(visited, 0, sizeof(visited));
    memset(rec_stack, 0, sizeof(rec_stack));

    // If P_i needs a resource held by P_j → edge i→j
    for (i = 0; i < state->num_processes; i++) {
        for (j = 0; j < state->num_processes; j++) {
            if (i != j) {
                int depends = 0;
                int k;
                for (k = 0; k < state->num_resources; k++) {
                    // P_i needs R_k and P_j holds R_k
                    if (state->need[i][k] > 0 && 
                        state->allocation[j][k] > 0) {
                        depends = 1;
                        break;
                    }
                }
                wait_for[i][j] = depends;
            }
        }
    }

    // DFS cycle detection
    int cycle_found = 0;
    for (i = 0; i < state->num_processes; i++) {
        if (!visited[i]) {
            // DFS from process i
            int stack[MAX_PROCESSES];
            int stack_top = 0;
            stack[stack_top++] = i;

            while (stack_top > 0) {
                int node = stack[--stack_top];
                if (!visited[node]) {
                    visited[node] = 1;
                    rec_stack[node] = 1;
                }
                for (j = 0; j < state->num_processes; j++) {
                    if (wait_for[node][j]) {
                        if (!visited[j]) {
                            stack[stack_top++] = j;
                        } else if (rec_stack[j]) {
                            cycle_found = 1;
                        }
                    }
                }
            }
        }
    }

    return cycle_found;
}

// Calculate Risk Level
void calculateRiskLevel(SystemState *state, PredictionResult *result) {
    int j;
    int total_available = 0;
    int total_resources = 0;

    for (j = 0; j < state->num_resources; j++) {
        total_available += state->available[j];
        total_resources += state->total[j];
    }

    // Calculate availability percentage
    float availability = 0;
    if (total_resources > 0)
        availability = (float)total_available / total_resources * 100;

    if (result->is_safe && availability > 30)
        result->risk_level = 0;      // 🟢 Safe
    else if (result->is_safe && availability <= 30)
        result->risk_level = 1;      // 🟡 Warning
    else
        result->risk_level = 2;      // 🔴 Danger
}

// Display Prediction Result
void displayPredictionResult(PredictionResult *result, int num_processes) {
    int i;

    printf("\n╔══════════════════════════════════════╗");
    printf("\n║         PREDICTION RESULTS           ║");
    printf("\n╚══════════════════════════════════════╝\n");

    // Risk level
    printf("\n🎯 Risk Level: ");
    if (result->risk_level == 0)
        printf("🟢 SAFE — System is in a safe state\n");
    else if (result->risk_level == 1)
        printf("🟡 WARNING — System resources running low\n");
    else
        printf("🔴 DANGER — Deadlock detected or imminent!\n");

    // Safe sequence
    if (result->is_safe) {
        printf("\n✅ Safe Sequence Found:\n   ");
        for (i = 0; i < num_processes; i++) {
            printf("P%d", result->safe_sequence[i]);
            if (i < num_processes - 1) printf(" → ");
        }
        printf("\n");
        printf("\n💡 Processes can complete in above order without deadlock.\n");
    } else {
        printf("\n🚨 DEADLOCK PREDICTED!\n");
        printf("   Deadlocked Processes: ");
        for (i = 0; i < result->num_deadlocked; i++)
            printf("P%d ", result->deadlocked_processes[i]);
        printf("\n");
        printf("\n💡 These processes are waiting for resources\n");
        printf("   that will never be released — DEADLOCK!\n");
    }
}