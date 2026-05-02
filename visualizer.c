#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "visualizer.h"

// ─── Welcome Banner ──────────────────────────────────────
void displayBanner() {
    printf("\n");
    printf("╔════════════════════════════════════════════════╗\n");
    printf("║                                                ║\n");
    printf("║     AUTOMATED DEADLOCK PREDICTION SYSTEM      ║\n");
    printf("║              OS Project — CSE316              ║\n");
    printf("║                                                ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("\n");
}

// ─── Main Menu ───────────────────────────────────────────
void displayMenu() {
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║                  MAIN MENU                     ║\n");
    printf("╠════════════════════════════════════════════════╣\n");
    printf("║  1. Display Current System State               ║\n");
    printf("║  2. Run Deadlock Prediction (Banker's)         ║\n");
    printf("║  3. Simulate a Resource Request                ║\n");
    printf("║  4. Display Resource Allocation Graph          ║\n");
    printf("║  5. Display Process Status                     ║\n");
    printf("║  6. Display Resource Status                    ║\n");
    printf("║  7. Save Results to File                       ║\n");
    printf("║  8. Exit                                       ║\n");
    printf("╚════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice: ");
}

// ─── Resource Allocation Graph ───────────────────────────
void displayRAG(SystemState *state) {
    int i, j;

    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║         RESOURCE ALLOCATION GRAPH              ║\n");
    printf("╚════════════════════════════════════════════════╝\n");

    printf("\n📌 Assignment Edges (Resource → Process):\n");
    printf("   (Shows what each process is currently holding)\n\n");

    int found = 0;
    for (i = 0; i < state->num_processes; i++) {
        for (j = 0; j < state->num_resources; j++) {
            if (state->allocation[i][j] > 0) {
                printf("   R%d ──(%d unit)──► P%d\n",
                       j, state->allocation[i][j], i);
                found = 1;
            }
        }
    }
    if (!found) printf("   No assignments currently.\n");

    printf("\n📌 Request Edges (Process → Resource):\n");
    printf("   (Shows what each process is waiting/needing)\n\n");

    found = 0;
    for (i = 0; i < state->num_processes; i++) {
        for (j = 0; j < state->num_resources; j++) {
            if (state->need[i][j] > 0) {
                printf("   P%d ──(%d unit)──► R%d\n",
                       i, state->need[i][j], j);
                found = 1;
            }
        }
    }
    if (!found) printf("   No pending requests.\n");

    printf("\n📌 Wait-For Graph:\n");
    printf("   (Shows which process is waiting for another)\n\n");

    found = 0;
    for (i = 0; i < state->num_processes; i++) {
        for (j = 0; j < state->num_processes; j++) {
            if (i != j) {
                int k, waits = 0;
                for (k = 0; k < state->num_resources; k++) {
                    if (state->need[i][k] > 0 &&
                        state->allocation[j][k] > 0) {
                        waits = 1;
                        break;
                    }
                }
                if (waits) {
                    printf("   P%d ──(waiting)──► P%d\n", i, j);
                    found = 1;
                }
            }
        }
    }
    if (!found) printf("   No wait-for dependencies.\n");
}

// ─── Summary Table ───────────────────────────────────────
void displaySummaryTable(SystemState *state, PredictionResult *result) {
    int i, j;

    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              SYSTEM SUMMARY TABLE              ║\n");
    printf("╚════════════════════════════════════════════════╝\n");

    // Header
    printf("\n%-6s", "P/R");
    for (j = 0; j < state->num_resources; j++)
        printf("  Alloc R%d  Need R%d", j, j);
    printf("\n");

    printf("──────");
    for (j = 0; j < state->num_resources; j++)
        printf("────────────────────");
    printf("\n");

    // Rows
    for (i = 0; i < state->num_processes; i++) {
        printf("P%-5d", i);
        for (j = 0; j < state->num_resources; j++)
            printf("     %d        %d    ",
                   state->allocation[i][j],
                   state->need[i][j]);
        printf("\n");
    }

    // Available row
    printf("\nAvailable Resources: ");
    for (j = 0; j < state->num_resources; j++)
        printf("R%d:[%d]  ", j, state->available[j]);

    // Status
    printf("\n\nSystem Status: ");
    if (result->risk_level == 0)
        printf("🟢 SAFE\n");
    else if (result->risk_level == 1)
        printf("🟡 WARNING\n");
    else
        printf("🔴 DEADLOCK DANGER\n");
}

// ─── Process Status ──────────────────────────────────────
void displayProcessStatus(SystemState *state, PredictionResult *result) {
    int i, j;

    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              PROCESS STATUS                    ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");

    for (i = 0; i < state->num_processes; i++) {
        // Check if process is deadlocked
        int deadlocked = 0;
        for (j = 0; j < result->num_deadlocked; j++) {
            if (result->deadlocked_processes[j] == i) {
                deadlocked = 1;
                break;
            }
        }

        

        printf("  Process P%d: ", i);
        if (deadlocked)
            printf("🔴 DEADLOCKED — waiting for resources\n");
        else if (result->is_safe)
            printf("🟢 ACTIVE — can complete successfully\n");
        else
            printf("🟡 WAITING — resource contention\n");

        // Show what it holds
        printf("    Holding: ");
        for (j = 0; j < state->num_resources; j++)
            printf("R%d:%d  ", j, state->allocation[i][j]);

        printf("\n    Needs:   ");
        for (j = 0; j < state->num_resources; j++)
            printf("R%d:%d  ", j, state->need[i][j]);
        printf("\n\n");
    }
}

// ─── Resource Status ─────────────────────────────────────
void displayResourceStatus(SystemState *state) {
    int i, j;

    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║              RESOURCE STATUS                   ║\n");
    printf("╚════════════════════════════════════════════════╝\n\n");

    for (j = 0; j < state->num_resources; j++) {
        int used = state->total[j] - state->available[j];
        float usage = (float)used / state->total[j] * 100;

        printf("  Resource R%d:\n", j);
        printf("    Total:     %d units\n", state->total[j]);
        printf("    Used:      %d units\n", used);
        printf("    Available: %d units\n", state->available[j]);
        printf("    Usage:     %.1f%%  ", usage);

        // Usage bar
        printf("[");
        int bar_length = 20;
        int filled = (int)(usage / 100 * bar_length);
        for (i = 0; i < bar_length; i++) {
            if (i < filled) printf("█");
            else printf("░");
        }
        printf("]\n\n");
    }
}

// ─── Save Results to File ────────────────────────────────
void saveResultsToFile(SystemState *state, PredictionResult *result) {
    int i, j;

    // Create output directory
    system("mkdir -p output");

    FILE *fp = fopen("output/results.txt", "w");
    if (fp == NULL) {
        printf("❌ Error: Could not create output file!\n");
        return;
    }

    fprintf(fp, "========================================\n");
    fprintf(fp, "  AUTOMATED DEADLOCK PREDICTION SYSTEM  \n");
    fprintf(fp, "========================================\n\n");

    fprintf(fp, "System Configuration:\n");
    fprintf(fp, "  Processes: %d\n", state->num_processes);
    fprintf(fp, "  Resources: %d\n\n", state->num_resources);

    fprintf(fp, "Allocation Matrix:\n");
    for (i = 0; i < state->num_processes; i++) {
        fprintf(fp, "  P%d: ", i);
        for (j = 0; j < state->num_resources; j++)
            fprintf(fp, "%d ", state->allocation[i][j]);
        fprintf(fp, "\n");
    }

    fprintf(fp, "\nNeed Matrix:\n");
    for (i = 0; i < state->num_processes; i++) {
        fprintf(fp, "  P%d: ", i);
        for (j = 0; j < state->num_resources; j++)
            fprintf(fp, "%d ", state->need[i][j]);
        fprintf(fp, "\n");
    }

    fprintf(fp, "\nAvailable Resources: ");
    for (j = 0; j < state->num_resources; j++)
        fprintf(fp, "R%d:%d  ", j, state->available[j]);

    fprintf(fp, "\n\nPrediction Result: ");
    if (result->is_safe) {
        fprintf(fp, "SAFE STATE\n");
        fprintf(fp, "Safe Sequence: ");
        for (i = 0; i < state->num_processes; i++)
            fprintf(fp, "P%d ", result->safe_sequence[i]);
    } else {
        fprintf(fp, "DEADLOCK DETECTED\n");
        fprintf(fp, "Deadlocked Processes: ");
        for (i = 0; i < result->num_deadlocked; i++)
            fprintf(fp, "P%d ", result->deadlocked_processes[i]);
    }

    fprintf(fp, "\n\nRisk Level: ");
    if (result->risk_level == 0) fprintf(fp, "SAFE\n");
    else if (result->risk_level == 1) fprintf(fp, "WARNING\n");
    else fprintf(fp, "DANGER\n");

    fclose(fp);
    printf("✅ Results saved to output/results.txt\n");
}