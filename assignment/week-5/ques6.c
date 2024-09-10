#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h> // for sleep function

#define SIZE 11

// Structure to hold the results of the square roots
typedef struct {
    double results[SIZE];
} sqrt_result_t;

// Function to be executed by the thread
void* compute_square_roots(void* arg) {
    sqrt_result_t* res = (sqrt_result_t*) arg;
    for (int i = 0; i < SIZE; i++) {
        res->results[i] = sqrt(i);
        sleep(1); // Simulate some time-consuming work
    }
    return NULL;
}

int main() {
    pthread_t thread;
    sqrt_result_t sqrt_result;

    // Create a separate thread to compute square roots
    if (pthread_create(&thread, NULL, compute_square_roots, &sqrt_result) != 0) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }

    // Main thread displays a short message
    printf("Computing square roots in a separate thread...\n");

    // Wait for the computation thread to finish
    if (pthread_join(thread, NULL) != 0) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    // Display the results after the computation is done
    printf("Square roots of integers from 0 to 10:\n");
    for (int i = 0; i < SIZE; i++) {
        printf("sqrt(%d) = %.2f\n", i, sqrt_result.results[i]);
    }

    return 0;
}
