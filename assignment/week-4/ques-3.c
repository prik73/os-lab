#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10
#define BURST_TIME 20

typedef struct {
    char name[10];
    int remaining_time;
    int consumed_time;
} Process;

int main() {
    int num_processes;
    Process processes[MAX_PROCESSES];
    int quantum;

    // Get the number of processes from the user
    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    // Read process names and assign burst time
    for (int i = 0; i < num_processes; i++) {
        printf("Enter name for process %d: ", i + 1);
        scanf("%s", processes[i].name);
        processes[i].remaining_time = BURST_TIME;
        processes[i].consumed_time = 0;
    }

    // Get the time quantum from the user
    printf("Enter the time quantum: ");
    scanf("%d", &quantum);

    // Apply Round Robin scheduling
    int current_process = 0;
    while (1) {
        int all_processes_completed = 1;
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].remaining_time > 0) {
                all_processes_completed = 0;
                if (processes[i].remaining_time <= quantum) {
                    processes[i].consumed_time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;
                    printf("Process %s completed. Total time consumed: %d ns\n", processes[i].name, processes[i].consumed_time);
                } else {
                    processes[i].remaining_time -= quantum;
                    processes[i].consumed_time += quantum;
                    printf("Process %s: Remaining time: %d ns, Total time consumed: %d ns\n", processes[i].name, processes[i].remaining_time, processes[i].consumed_time);
                }
            }
        }
        if (all_processes_completed) {
            break;
        }
        current_process = (current_process + 1) % num_processes;
    }

    return 0;
}