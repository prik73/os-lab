#include <stdio.h>

typedef struct {
    int pid;            // Process ID
    int burst_time;     // Burst time
    int waiting_time;   // Waiting time
    int turnaround_time; // Turnaround time
} Process;

void calculateWaitingTime(Process processes[], int n) {
    processes[0].waiting_time = 0; // First process has no waiting time

    for (int i = 1; i < n; i++) {
        processes[i].waiting_time = processes[i - 1].waiting_time + processes[i - 1].burst_time;
    }
}

void calculateTurnaroundTime(Process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].turnaround_time = processes[i].burst_time + processes[i].waiting_time;
    }
}

void sortProcessesByBurstTime(Process processes[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].burst_time > processes[j + 1].burst_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;
    Process processes[100];

    // Read number of processes
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    // Read burst time for all processes
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1; // Assigning Process ID
        printf("Enter burst time for process %d: ", processes[i].pid);
        scanf("%d", &processes[i].burst_time);
    }

    // Sort processes by burst time
    sortProcessesByBurstTime(processes, n);

    // Calculate waiting time and turnaround time
    calculateWaitingTime(processes, n);
    calculateTurnaroundTime(processes, n);

    // Print process information
    printf("\nProcess\tBurst Time\tWaiting Time\tTurnaround Time\n");
    printf("-------\t-----------\t-------------\t-----------------\n");

    int total_waiting_time = 0;
    int total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
    }

    // Calculate average waiting time and turnaround time
    float average_waiting_time = (float)total_waiting_time / n;
    float average_turnaround_time = (float)total_turnaround_time / n;

    // Print averages
    printf("\nAverage Waiting Time: %.2f sec", average_waiting_time);
    printf("\nAverage Turnaround Time: %.2f sec\n", average_turnaround_time);

    return 0;
}