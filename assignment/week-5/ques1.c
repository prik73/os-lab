#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Function that each thread will execute
void* print_hello(void* threadid) {
    long tid = (long)threadid; // Cast threadid to long
    printf("Hello World from Thread %ld!\n", tid);
    pthread_exit(NULL); // Terminate the thread
}

int main() {
    pthread_t threads[5]; // Array to hold thread identifiers
    int rc;
    long t;

    // Create 5 threads
    for (t = 0; t < 5; t++) {
        rc = pthread_create(&threads[t], NULL, print_hello, (void*)t);
        if (rc) {
            fprintf(stderr, "Error: Unable to create thread %ld, %d\n", t, rc);
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (t = 0; t < 5; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("All threads completed successfully.\n");
    return 0;
}