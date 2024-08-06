#include <stdio.h>
#include <unistd.h>

int main() {
    // Get the nice value of the current process
    int nice_value = nice(0);

    // Print the nice value
    printf("Nice Number: %d\n", nice_value);

    return 0;
}
