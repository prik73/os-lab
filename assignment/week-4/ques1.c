#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int burst_time;
    int priority;
} Process;

int main() {
    int num_processes;
    Process processes[MAX_PROCESSES];

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    // Read burst time and priority for all processes
    for (int i = 0; i < num_processes; i++) {
        processes[i].pid = i + 1;
        printf("Enter burst time and priority for process %d: ", processes[i].pid);
        scanf("%d %d", &processes[i].burst_time, &processes[i].priority);
    }

    // Apply priority scheduling algorithm
    for (int i = 0; i < num_processes - 1; i++) {
        for (int j = 0; j < num_processes - i - 1; j++) {
            if (processes[j].priority > processes[j + 1].priority) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Print PID, Burst Time, Priority of process and Gantt Chart
    printf("\nPID\tBurst Time\tPriority\n");
    printf("---\t-----------\t--------\n");

    int total_time = 0;
    for (int i = 0; i < num_processes; i++) {
        printf("%d\t%d\t\t%d\n", processes[i].pid, processes[i].burst_time, processes[i].priority);
        total_time += processes[i].burst_time;
    }

    printf("\nGantt Chart:\n");
    for (int i = 0; i < num_processes; i++) {
        printf("P%d ", processes[i].pid);
    }
    printf("\n");

    for (int i = 0; i < total_time; i++) {
        printf("--");
    }
    printf("\n");

    int start_time = 0;
    for (int i = 0; i < num_processes; i++) {
        printf("|");
        for (int j = 0; j < processes[i].burst_time - 1; j++) {
            printf(" ");
        }
        printf("%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst_time - 1; j++) {
            printf(" ");
        }
        printf("|");
        start_time += processes[i].burst_time;
    }
    printf("\n");

    for (int i = 0; i < total_time; i++) {
        printf("--");
    }
    printf("\n");

    return 0;
}