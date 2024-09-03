#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork()

int main() {
    pid_t pid; // Variable to store process ID

    // Create a new process
    pid = fork();

    if (pid < 0) {
        // Error occurred
        perror("Fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process
        printf("Hello from the Child Process! My PID is %d\n", getpid());
    } else {
        // Parent process
        printf("Hello from the Parent Process! My PID is %d and my child's PID is %d\n", getpid(), pid);
    }

    // Both processes will execute this line
    printf("This is printed from both processes. PID: %d\n", getpid());

    return 0;
}