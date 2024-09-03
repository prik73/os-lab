#include <stdio.h>
#include <unistd.h>

int main() {
    // Get the terminal name associated with standard input (file descriptor 0)
    char *tty_name = ttyname(0);

    // Check if ttyname returned a valid name
    if (tty_name != NULL) {
        printf("Terminal (TTY): %s\n", tty_name);
    } else {
        printf("No terminal associated with this process.\n");
    }

    return 0;
}
