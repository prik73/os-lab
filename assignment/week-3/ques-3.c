#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void alarm_handler(int signum) {
    printf("\a"); // Beep sound
    printf("Time's up! Your timer has finished.\n");
    exit(0);
}

int main() {
    int seconds;
    printf("Enter the time in seconds: ");
    scanf("%d", &seconds);

    // Set up the alarm signal handler
    signal(SIGALRM, alarm_handler);

    // Set the alarm to go off after the specified number of seconds
    alarm(seconds);

    printf("Timer started. You can execute other programs now.\n");

    // Wait for the alarm signal
    while (1) {
        pause();
    }

    return 0;
}