#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"
#include "predictor.h"
#include "visualizer.h"
#include "utils.h"

int main() {
    // ─── Declare Variables ───────────────────────────────
    SystemState state;
    PredictionResult result;
    int choice;
    int process_id;
    int request[MAX_RESOURCES];
    int i;

    // ─── Welcome Screen ──────────────────────────────────
    clearScreen();
    displayBanner();

    // ─── Initialize System ───────────────────────────────
    printLoading("Initializing system");
    initializeSystem(&state);
    resetPredictionResult(&result);

    // ─── Run Initial Prediction ──────────────────────────
    printLoading("Running initial deadlock analysis");
    isSafeState(&state, &result);

    // ─── Main Menu Loop ──────────────────────────────────
    while (1) {
        clearScreen();
        displayBanner();
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {

            // ── Option 1: Display System State ───────────
            case 1:
                clearScreen();
                displayState(&state);
                pauseScreen();
                break;

            // ── Option 2: Run Deadlock Prediction ────────
            case 2:
                clearScreen();
                printLoading("Running Banker's Algorithm");
                resetPredictionResult(&result);
                isSafeState(&state, &result);
                displayPredictionResult(&result, state.num_processes);
                printDivider();
                displaySummaryTable(&state, &result);

                // Also check for cycles
                printDivider();
                printf("\n🔍 Checking for cycles in RAG...\n");
                int cycle = detectCycle(&state, &result);
                if (cycle)
                    printf("🔴 CYCLE DETECTED in Resource Allocation Graph!\n");
                else
                    printf("🟢 No cycles detected in Resource Allocation Graph.\n");

                pauseScreen();
                break;

            // ── Option 3: Simulate Resource Request ──────
            case 3:
                clearScreen();
                printf("\n╔════════════════════════════════════════════════╗\n");
                printf("║           SIMULATE RESOURCE REQUEST            ║\n");
                printf("╚════════════════════════════════════════════════╝\n");

                // Get process id
                printf("\nEnter Process ID (0 to %d): ",
                       state.num_processes - 1);
                scanf("%d", &process_id);

                if (!validateInput(process_id, 0,
                                   state.num_processes - 1)) {
                    pauseScreen();
                    break;
                }

                // Get request amounts
                printf("\nEnter resource request for P%d:\n", process_id);
                for (i = 0; i < state.num_resources; i++) {
                    printf("  Request units of R%d: ", i);
                    scanf("%d", &request[i]);
                }

                // Predict if request is safe
                printDivider();
                resetPredictionResult(&result);
                predictRequest(&state, process_id, request, &result);
                displayPredictionResult(&result, state.num_processes);
                pauseScreen();
                break;

            // ── Option 4: Display RAG ─────────────────────
            case 4:
                clearScreen();
                displayRAG(&state);
                pauseScreen();
                break;

            // ── Option 5: Process Status ──────────────────
            case 5:
                clearScreen();
                resetPredictionResult(&result);
                isSafeState(&state, &result);
                displayProcessStatus(&state, &result);
                pauseScreen();
                break;

            // ── Option 6: Resource Status ─────────────────
            case 6:
                clearScreen();
                displayResourceStatus(&state);
                pauseScreen();
                break;

            // ── Option 7: Save Results ────────────────────
            case 7:
                clearScreen();
                resetPredictionResult(&result);
                isSafeState(&state, &result);
                saveResultsToFile(&state, &result);
                pauseScreen();
                break;

            // ── Option 8: Exit ────────────────────────────
            case 8:
                clearScreen();
                printf("\n╔════════════════════════════════════════════════╗\n");
                printf("║       Thank you for using DeadlockPredictor    ║\n");
                printf("║              Goodbye! 👋                       ║\n");
                printf("╚════════════════════════════════════════════════╝\n\n");
                exit(0);

            // ── Invalid Choice ────────────────────────────
            default:
                printError("Invalid choice! Please select 1-8");
                pauseScreen();
                break;
        }
    }

    return 0;
}