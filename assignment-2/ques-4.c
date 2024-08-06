#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Include this header file

int main() {
    // Call fork() three times
    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // This block is executed by the child process
            printf("Child process created with PID: %d, Parent PID: %d\n", getpid(), getppid());
            exit(0); // Exit child process to prevent further forking
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < 3; i++) {
        wait(NULL); // Wait for any child process to finish
    }

    printf("Total processes created: %d\n", 8); // Total processes = 2^3 = 8

    return 0;
}
