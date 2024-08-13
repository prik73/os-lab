#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100
#define ADDITIONAL_STRING " World!"

int main() {
    int pipe1[2]; // Pipe for P1 to P2
    int pipe2[2]; // Pipe for P2 to P1
    pid_t pid;

    // Create the first pipe
    if (pipe(pipe1) == -1) {
        perror("pipe1");
        exit(EXIT_FAILURE);
    }

    // Create the second pipe
    if (pipe(pipe2) == -1) {
        perror("pipe2");
        exit(EXIT_FAILURE);
    }

    // Create the child process P2
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent process (P1)
        close(pipe1[0]); // Close unused read end of pipe1
        close(pipe2[1]); // Close unused write end of pipe2

        char input_string[BUFFER_SIZE];
        printf("Enter a string: ");
        fgets(input_string, BUFFER_SIZE, stdin);
        input_string[strcspn(input_string, "\n")] = 0; // Remove newline character

        // Send the string to P2
        write(pipe1[1], input_string, strlen(input_string) + 1);
        close(pipe1[1]); // Close write end after sending

        // Read the concatenated string from P2
        char output_string[BUFFER_SIZE];
        read(pipe2[0], output_string, BUFFER_SIZE);
        close(pipe2[0]); // Close read end after receiving

        printf("Received from P2: %s\n", output_string);
    } else { // Child process (P2)
        close(pipe1[1]); // Close unused write end of pipe1
        close(pipe2[0]); // Close unused read end of pipe2

        char received_string[BUFFER_SIZE];
        // Read the string from P1
        read(pipe1[0], received_string, BUFFER_SIZE);
        close(pipe1[0]); // Close read end after receiving

        // Concatenate the additional string without using string functions
        char concatenated_string[BUFFER_SIZE];
        int i = 0, j = 0;

        // Copy received string to concatenated_string
        while (received_string[i] != '\0') {
            concatenated_string[i] = received_string[i];
            i++;
        }

        // Copy additional string to concatenated_string
        while (ADDITIONAL_STRING[j] != '\0') {
            concatenated_string[i] = ADDITIONAL_STRING[j];
            i++;
            j++;
        }
        concatenated_string[i] = '\0'; // Null-terminate the concatenated string

        // Send the concatenated string back to P1
        write(pipe2[1], concatenated_string, strlen(concatenated_string) + 1);
        close(pipe2[1]); // Close write end after sending
    }

    return 0;
}