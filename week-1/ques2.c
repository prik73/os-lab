#include <stdio.h>

int main() {
    int n, i;
    float sum = 0.0, average;

    // Prompt user to enter the number of elements
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    // Allocate an array to store the integers
    int array[n];

    // Read the integers from the user
    printf("Enter %d integers:\n", n);
    for (i = 0; i < n; i++) {
        scanf("%d", &array[i]);
        sum += array[i];
    }

    // Calculate the average
    average = sum / n;

    // Print the result
    printf("Average of the entered integers: %.2f\n", average);

    return 0;
}
