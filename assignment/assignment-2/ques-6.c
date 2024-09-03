    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>

    int main() {
        int pipe1[2]; // Pipe for addition and subtraction
        int pipe2[2]; // Pipe for multiplication and division
        int num1 = 10, num2 = 5;

        // Create the first pipe
        if (pipe(pipe1) == -1) {
            perror("Pipe 1 failed");
            exit(1);
        }

        // Create the second pipe
        if (pipe(pipe2) == -1) {
            perror("Pipe 2 failed");
            exit(1);
        }

        // Fork the first child for addition
        pid_t add_pid = fork();
        if (add_pid < 0) {
            perror("Fork failed for addition");
            exit(1);
        }

        if (add_pid == 0) {
            // Child process for addition
            close(pipe1[0]); // Close read end
            int result = num1 + num2;
            write(pipe1[1], &result, sizeof(result)); // Write result to pipe
            close(pipe1[1]); // Close write end
            exit(0);
        }

        // Fork the second child for subtraction
        pid_t sub_pid = fork();
        if (sub_pid < 0) {
            perror("Fork failed for subtraction");
            exit(1);
        }

        if (sub_pid == 0) {
            // Child process for subtraction
            close(pipe1[0]); // Close read end
            int result = num1 - num2;
            write(pipe1[1], &result, sizeof(result)); // Write result to pipe
            close(pipe1[1]); // Close write end
            exit(0);
        }

        // Wait for addition and subtraction processes to complete
        wait(NULL);
        wait(NULL);

        // Read results from the first pipe
        int add_result, sub_result;
        close(pipe1[1]); // Close write end
        read(pipe1[0], &add_result, sizeof(add_result));
        read(pipe1[0], &sub_result, sizeof(sub_result));
        close(pipe1[0]); // Close read end

        printf("Addition Result: %d\n", add_result);
        printf("Subtraction Result: %d\n", sub_result);

        // Fork the third child for multiplication
        pid_t mul_pid = fork();
        if (mul_pid < 0) {
            perror("Fork failed for multiplication");
            exit(1);
        }

        if (mul_pid == 0) {
            // Child process for multiplication
            close(pipe2[0]); // Close read end
            int result = num1 * num2;
            write(pipe2[1], &result, sizeof(result)); // Write result to pipe
            close(pipe2[1]); // Close write end
            exit(0);
        }

        // Fork the fourth child for division
        pid_t div_pid = fork();
        if (div_pid < 0) {
            perror("Fork failed for division");
            exit(1);
        }

        if (div_pid == 0) {
            // Child process for division
            close(pipe2[0]); // Close read end
            if (num2 == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            int result = num1 / num2;
            write(pipe2[1], &result, sizeof(result)); // Write result to pipe
            close(pipe2[1]); // Close write end
            exit(0);
        }

        // Wait for multiplication and division processes to complete
        wait(NULL);
        wait(NULL);

        // Read results from the second pipe
        int mul_result, div_result;
        close(pipe2[1]); // Close write end
        read(pipe2[0], &mul_result, sizeof(mul_result));
        read(pipe2[0], &div_result, sizeof(div_result));
        close(pipe2[0]); // Close read end

        printf("Multiplication Result: %d\n", mul_result);
        printf("Division Result: %d\n", div_result);

        return 0;
    }
