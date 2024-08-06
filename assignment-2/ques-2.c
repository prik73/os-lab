#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    // Get the process ID of the current process
    pid_t pid = getpid();

    // Print the message before forking
    printf("Before fork: %d\n", pid);

    // Create a new process using fork
    pid_t child_pid = fork();

    if (child_pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    } else if (child_pid == 0) {
        // This block is executed by the child process
        printf("Child: %d\n", getpid());
        printf("I am the child: %d\n", getpid());
    } else {
        // This block is executed by the parent process
        printf("I am the father: %d of %d\n", pid, child_pid);
    }

    return 0;
}
