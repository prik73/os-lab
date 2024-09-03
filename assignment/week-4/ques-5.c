#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 10
#define MAX_NAME_LENGTH 20
#define TIME_QUANTUM 5

typedef struct {
    char name[MAX_NAME_LENGTH];
    int burst_time;
    int remaining_time;
    int wait_time;
    int turnaround_time;
} Process;

// Function prototypes
void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void round_robin(Process processes[], int n, int time_quantum);
void mlfq(Process processes[], int n);
void calculate_average_times(Process processes[], int n);

int main() {
    int n;
    Process processes[MAX_PROCESSES];

    // Input number of processes
    printf("Enter the number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    // Input process details
    for (int i = 0; i < n; i++) {
        printf("Enter name and burst time for process %d: ", i + 1);
        scanf("%s %d", processes[i].name, &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time; // Initialize remaining time
        processes[i].wait_time = 0; // Initialize wait time
        processes[i].turnaround_time = 0; // Initialize turnaround time
    }

    int choice;
    do {
        printf("\n--- Multilevel Feedback Queue Scheduling ---\n");
        printf("1. FCFS Scheduling\n");
        printf("2. SJF Scheduling\n");
        printf("3. Round Robin Scheduling\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(processes, n);
                break;
            case 2:
                sjf(processes, n);
                break;
            case 3:
                round_robin(processes, n, TIME_QUANTUM);
                break;
            case 4:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);

    return 0;
}

void fcfs(Process processes[], int n) {
    printf("\n--- FCFS Scheduling ---\n");
    int total_wait_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        if (i == 0) {
            processes[i].wait_time = 0; // First process has no waiting time
        } else {
            processes[i].wait_time = processes[i - 1].wait_time + processes[i - 1].burst_time;
        }
        processes[i].turnaround_time = processes[i].wait_time + processes[i].burst_time;

        total_wait_time += processes[i].wait_time;
        total_turnaround_time += processes[i].turnaround_time;

        printf("Process %s: Wait Time: %d, Turnaround Time: %d\n", processes[i].name, processes[i].wait_time, processes[i].turnaround_time);
    }

    printf("Average Wait Time: %.2f sec\n", (float)total_wait_time / n);
    printf("Average Turnaround Time: %.2f sec\n", (float)total_turnaround_time / n);
}

void sjf(Process processes[], int n) {
    printf("\n--- SJF Scheduling ---\n");

    // Sort processes by burst time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    int total_wait_time = 0, total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        if (i == 0) {
            processes[i].wait_time = 0; // First process has no waiting time
        } else {
            processes[i].wait_time = processes[i - 1].wait_time + processes[i - 1].burst_time;
        }
        processes[i].turnaround_time = processes[i].wait_time + processes[i].burst_time;

        total_wait_time += processes[i].wait_time;
        total_turnaround_time += processes[i].turnaround_time;

        printf("Process %s: Wait Time: %d, Turnaround Time: %d\n", processes[i].name, processes[i].wait_time, processes[i].turnaround_time);
    }

    printf("Average Wait Time: %.2f sec\n", (float)total_wait_time / n);
    printf("Average Turnaround Time: %.2f sec\n", (float)total_turnaround_time / n);
}

void round_robin(Process processes[], int n, int time_quantum) {
    printf("\n--- Round Robin Scheduling ---\n");
    int total_wait_time = 0, total_turnaround_time = 0;
    int remaining_processes = n;
    int time = 0;

    while (remaining_processes > 0) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0) {
                if (processes[i].remaining_time > time_quantum) {
                    time += time_quantum;
                    processes[i].remaining_time -= time_quantum;
                } else {
                    time += processes[i].remaining_time;
                    processes[i].wait_time = time - processes[i].burst_time;
                    processes[i].turnaround_time = time;
                    processes[i].remaining_time = 0;
                    printf("Process %s completed at time %d\n", processes[i].name, time);
                    remaining_processes--;
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {
        total_wait_time += processes[i].wait_time;
        total_turnaround_time += processes[i].turnaround_time;
        printf("Process %s: Wait Time: %d, Turnaround Time: %d\n", processes[i].name, processes[i].wait_time, processes[i].turnaround_time);
    }

    printf("Average Wait Time: %.2f sec\n", (float)total_wait_time / n);
    printf("Average Turnaround Time: %.2f sec\n", (float)total_turnaround_time / n);
}