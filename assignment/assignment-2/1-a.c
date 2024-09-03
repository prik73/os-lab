#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Process ID (PID): %d\n", getpid());
    return 0;
}