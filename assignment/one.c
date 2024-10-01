#include <stdio.h>
#include <stdbool.h>

// Function to check if a process can be executed
bool canExecute(int P, int R, int need[P][R], int available[R], int process) {
    for (int j = 0; j < R; j++) {
        if (need[process][j] > available[j]) {
            return false;
        }
    }
    return true;
}

// Function to check if the system is in a safe state
bool isSafe(int P, int R, int allocation[P][R], int max[P][R], int available[R]) {
    int work[R];
    bool finish[P];

    // Initialize work and finish arrays
    for (int i = 0; i < R; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < P; i++) {
        finish[i] = false;
    }

    // Calculate the need matrix
    int need[P][R];
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Check for a safe sequence
    int safeSequence[P];
    int count = 0;

    while (count < P) {
        bool foundProcess = false;

        for (int i = 0; i < P; i++) {
            if (!finish[i] && canExecute(P, R, need, work, i)) {
                // Process i can execute, update work and finish arrays
                for (int j = 0; j < R; j++) {
                    work[j] += allocation[i][j];
                }

                safeSequence[count++] = i;
                finish[i] = true;
                foundProcess = true;
            }
        }

        if (!foundProcess) {
            return false; // The system is not in a safe state
        }
    }

    // Display the safe sequence
    printf("The system is in a safe state.\nSafe sequence: ");
    for (int i = 0; i < P; i++) {
        printf("P%d ", safeSequence[i]);
    }
    printf("\n");

    return true;
}

int main() {
    int P, R;

    // Input the number of processes and resource types
    printf("Enter the number of processes (P): ");
    scanf("%d", &P);
    printf("Enter the number of resource types (R): ");
    scanf("%d", &R);

    int max[P][R], allocation[P][R], available[R];

    // Input the Maximum resource matrix
    printf("Enter the maximum resource matrix:\n");
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // Input the Allocation matrix
    printf("Enter the allocation matrix:\n");
    for (int i = 0; i < P; i++) {
        for (int j = 0; j < R; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    // Input the Available resources vector
    printf("Enter the available resources vector:\n");
    for (int i = 0; i < R; i++) {
        scanf("%d", &available[i]);
    }

    // Check if the system is in a safe state
    if (!isSafe(P, R, allocation, max, available)) {
        printf("The system is not in a safe state.\n");
    }

    return 0;
}
