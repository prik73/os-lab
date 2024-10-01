#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 10
struct Process {
int pid;
int burst_time;
int priority;
int waiting_time;
int turnaround_time;
};
// Comparison function to sort processes by priority
int compareByPriority(const void *a, const void *b) {
struct Process *p1 = (struct Process *)a;
struct Process *p2 = (struct Process *)b;
return p1->priority - p2->priority;
}
// Function to calculate waiting time and turnaround time
void calculateTimes(struct Process processes[], int n) {
processes[0].waiting_time = 0;
processes[0].turnaround_time = processes[0].burst_time;
for (int i = 1; i < n; ++i) {
processes[i].waiting_time = processes[i - 1].waiting_time + processes[i - 1].burst_time;
processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
}
}
// Function to print Gantt chart
void printGanttChart(struct Process processes[], int n) {
printf("\nGantt Chart:\n");
printf("0\t");
for (int i = 0; i < n; ++i) {
printf("|P%d\t", processes[i].pid);
}
printf("|\n");int time = 0;
printf("%d\t", time);
for (int i = 0; i < n; ++i) {
time += processes[i].burst_time;
printf("%d\t", time);
}
printf("\n");
}
int main() {
int n;
printf("Enter the number of processes: ");
scanf("%d", &n);
if (n > MAX_PROCESSES) {
printf("Number of processes exceeds maximum limit (%d).\n", MAX_PROCESSES);
return 1;
}
struct Process processes[n];
// Input burst times and priorities
for (int i = 0; i < n; ++i) {
printf("Enter burst time and priority for process %d: ", i + 1);
scanf("%d %d", &processes[i].burst_time, &processes[i].priority);
processes[i].pid = i + 1; // Assigning process ID
}
// Sort processes by priority
qsort(processes, n, sizeof(struct Process), compareByPriority);
// Calculate waiting time and turnaround time
calculateTimes(processes, n);
// Print process details
printf("\nPID\tBurst Time\tPriority\tWaiting Time\tTurnaround Time\n");
for (int i = 0; i < n; ++i) {
printf("%d\t%d\t\t%d\t\t%d\t\t%d\n",
processes[i].pid,
processes[i].burst_time,
processes[i].priority,processes[i].waiting_time,
processes[i].turnaround_time);
}
// Print Gantt chart
printGanttChart(processes, n);
return 0;
}