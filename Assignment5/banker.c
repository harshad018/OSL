#include <stdio.h>

int np, nr;
int allocation[10][10], max_matrix[10][10], need[10][10];
int available[10];
int safe_sequence[10];
int completed[10];

void calculateNeed() {
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < nr; j++) {
            need[i][j] = max_matrix[i][j] - allocation[i][j];
        }
    }
}

int main() {
    // User input for the number of processes and resources
    printf("Enter the number of processes: ");
    scanf("%d", &np);
    printf("Enter the number of resources: ");
    scanf("%d", &nr);

    // User input for the Allocation matrix
    printf("Enter the Allocation matrix (values row by row):\n");
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < nr; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // User input for the Max matrix
    printf("Enter the Max matrix (values row by row):\n");
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < nr; j++) {
            scanf("%d", &max_matrix[i][j]);
        }
    }

    // User input for the Available resources
    printf("Enter the Available resources (values in a single line):\n");
    for (int i = 0; i < nr; i++) {
        scanf("%d", &available[i]);
    }

    // Initialize the Need matrix based on Allocation and Max
    calculateNeed();

    // Initialize completed array to track completed processes
    for (int i = 0; i < np; i++) {
        completed[i] = 0;
    }

    int count = 0;      // Count of processes in safe sequence
    int canProceed = 1; // Flag to detect if any process completed in an iteration

    while (count < np && canProceed) {
        canProceed = 0;
        for (int i = 0; i < np; i++) {
            if (completed[i] == 0) { // Process not yet completed
                int canAllocate = 1;
                for (int j = 0; j < nr; j++) {
                    if (need[i][j] > available[j]) {
                        canAllocate = 0;
                        break;
                    }
                }
                if (canAllocate) {
                    // If resources can be allocated to process i, update available resources
                    for (int j = 0; j < nr; j++) {
                        available[j] += allocation[i][j];
                    }
                    safe_sequence[count++] = i; // Record process in safe sequence
                    completed[i] = 1;            // Mark process as completed
                    canProceed = 1;
                }
            }
        }
    }

    // Output the results
    if (count == np) {
        printf("System is in a safe state\n");
        printf("Safe Sequence: ");
        for (int i = 0; i < np; i++) {
            printf("P%d ", safe_sequence[i]);
        }
        printf("\n");
    } else {
        printf("System is not in a safe state\n");
    }

    return 0;
}
