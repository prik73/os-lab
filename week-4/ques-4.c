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
    int priority; // Lower number indicates higher priority
} Process;

// Function prototypes
void fcfs(Process processes[], int n);
void sjf(Process processes[], int n);
void round_robin(Process processes[], int n, int time_quantum);
void mlfq(Process processes[], int n);

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
        processes[i].priority = i; // Assign priority based on order of input
    }

    // Start MLFQ scheduling
    mlfq(processes, n);

    return 0;
}

void fcfs(Process processes[], int n) {
    printf("\n--- FCFS Scheduling ---\n");
    int wait_time = 0, total_wait_time = 0;

    for (int i = 0; i < n; i++) {
        printf("Process %s: Wait Time: %d, Burst Time: %d\n", processes[i].name, wait_time, processes[i].burst_time);
        total_wait_time += wait_time;
        wait_time += processes[i].burst_time;
    }

    printf("Average Wait Time: %.2f\n", (float)total_wait_time / n);
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

    int wait_time = 0, total_wait_time = 0;

    for (int i = 0; i < n; i++) {
        printf("Process %s: Wait Time: %d, Burst Time: %d\n", processes[i].name, wait_time, processes[i].burst_time);
        total_wait_time += wait_time;
        wait_time += processes[i].burst_time;
    }

    printf("Average Wait Time: %.2f\n", (float)total_wait_time / n);
}

void round_robin(Process processes[], int n, int time_quantum) {
    printf("\n--- Round Robin Scheduling ---\n");
    int wait_time = 0, total_wait_time = 0;
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
                    wait_time += time - processes[i].burst_time;
                    processes[i].remaining_time = 0;
                    printf("Process %s completed at time %d\n", processes[i].name, time);
                    remaining_processes--;
                }
            }
        }
    }

    printf("Total Wait Time: %d\n", wait_time);
    printf("Average Wait Time: %.2f\n", (float)wait_time / n);
}

void mlfq(Process processes[], int n) {
    // For simplicity, we will use a fixed strategy for MLFQ
    printf("\n--- Multilevel Feedback Queue Scheduling ---\n");
    
    // First Queue - FCFS
    fcfs(processes, n);

    // Second Queue - SJF
    sjf(processes, n);

    // Third Queue - Round Robin
    round_robin(processes, n, TIME_QUANTUM);
}