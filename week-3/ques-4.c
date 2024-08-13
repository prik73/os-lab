#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int main() {
    pid_t pid;
    int status;

    for (int i = 0; i < 4; i++) {
        pid = fork();
        if (pid == 0) { // Child process
            FILE *file;
            char buffer[BUFFER_SIZE];

            file = fopen("my_file.txt", "r");
            if (file == NULL) {
                perror("Error opening file");
                exit(EXIT_FAILURE);
            }

            // Move the file pointer to the appropriate line
            fseek(file, i * BUFFER_SIZE, SEEK_SET);

            // Read the line
            fgets(buffer, BUFFER_SIZE, file);

            printf("%s", buffer);
            fclose(file);
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < 4; i++) {
        pid = wait(&status);
        if (pid == -1) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}