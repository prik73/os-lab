#include <stdio.h>
#include <unistd.h>

int main() {
    // Get the parent process ID
    pid_t ppid = getppid();

    // Print the parent process ID
    printf("Parent Process ID (PPID): %d\n", ppid);

    return 0;
}