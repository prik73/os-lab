#include <stdio.h>
#include <pthread.h>
#include <math.h>    // For sin and tan functions
#include <stdlib.h>  // For exit
#include <unistd.h>  // For sleep

#define NUM_THREADS 4

// Thread data structure to store thread number and result
typedef struct {
    int thread_id;
    double result;
} thread_data_t;

void *calculate_sum(void *threadarg) {
    thread_data_t *data = (thread_data_t *)threadarg;
    int tid = data->thread_id;
    
    printf("Thread %d starting...\n", tid);
    
    double sum = 0;
    for (int i = 0; i <= 1000; ++i) {
        sum += sin(i) + tan(i);
    }
    
    data->result = sum;
    
    printf("Thread %d done. Result = %e\n", tid, sum);
    
    // Instead of casting the integer to a void pointer, pass the address of tid
    pthread_exit((void *)(intptr_t)tid);  // intptr_t ensures portability across systems
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    int rc;
    void *status;
    thread_data_t thread_data[NUM_THREADS];

    // Initialize and set thread attributes to joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Main: creating thread %d\n", i);
        thread_data[i].thread_id = i;
        rc = pthread_create(&threads[i], &attr, calculate_sum, (void *)&thread_data[i]);
        if (rc) {
            printf("Error: unable to create thread %d\n", rc);
            exit(-1);
        }
        sleep(1);  // Simulate a slight delay between thread creation
    }

    // Free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(threads[i], &status);
        if (rc) {
            printf("Error: unable to join thread %d\n", i);
            exit(-1);
        }
        // Convert the status back to an integer
        printf("Main: completed join with thread %d having a status of %ld\n", i, (long)(intptr_t)status);
    }

    printf("Main: program completed. Exiting.\n");
    pthread_exit(NULL);
}
