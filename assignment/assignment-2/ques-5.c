#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int num1 = 20, num2 = 5;

    // Fork child process for addition
    pid_t add_pid = fork();
    if (add_pid == 0) {
        // Child process for addition
        int result = num1 + num2;
        printf("Addition: %d + %d = %d\n", num1, num2, result);
        exit(0);
    }

    // Fork child process for subtraction
    pid_t sub_pid = fork();
    if (sub_pid == 0) {
        // Child process for subtraction
        int result = num1 - num2;
        printf("Subtraction: %d - %d = %d\n", num1, num2, result);
        exit(0);
    }

    // Fork child process for multiplication
    pid_t mul_pid = fork();
    if (mul_pid == 0) {
        // Child process for multiplication
        int result = num1 * num2;
        printf("Multiplication: %d * %d = %d\n", num1, num2, result);
        exit(0);
    }

    // Fork child process for division
    pid_t div_pid = fork();
    if (div_pid == 0) {
        // Child process for division
        if (num2 == 0) {
            printf("Error: Division by zero\n");
            exit(1);
        }
        int result = num1 / num2;
        printf("Division: %d / %d = %d\n", num1, num2, result);
        exit(0);
    }

    // Parent process waits for all child processes to complete
    waitpid(add_pid, NULL, 0);
    waitpid(sub_pid, NULL, 0);
    waitpid(mul_pid, NULL, 0);
    waitpid(div_pid, NULL, 0);

    printf("All child processes have completed their tasks.\n");

    return 0;
}
