#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child_pid;

    // Create a new process using fork
    child_pid = fork();

    if (child_pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    } else if (child_pid == 0) {
        // This block is executed by the child process
        printf("Child process (PID: %d) is running...\n", getpid());
        sleep(5); // Simulate some work in the child process
        printf("Child process (PID: %d) is exiting...\n", getpid());
        exit(0); // Child exits
    } else {
        // This block is executed by the parent process
        printf("Parent process (PID: %d) is exiting...\n", getpid());
        // Parent does not wait for the child to exit, creating a zombie process
        exit(0);
    }

    return 0;
}
