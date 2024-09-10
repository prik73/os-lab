#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<unistd.h>

#define COUNT_LIMIT 10  // Predefined limit for the count
#define TCOUNT 20       // Total count value

int count = 0;  // Shared variable
pthread_mutex_t count_mutex;  // Mutex to protect the shared variable
pthread_cond_t count_threshold_cv;  // Condition variable

// Thread that increments the count
void *inc_count(void *idp) {
    int my_id = *((int *)idp);

    for (int i = 0; i < TCOUNT; i++) {
        pthread_mutex_lock(&count_mutex);  // Lock the mutex

        count++;

        // Check if count has reached the predefined limit
        if (count == COUNT_LIMIT) {
            printf("inc_count(): Thread %d, count = %d. Threshold reached.\n", my_id, count);
            pthread_cond_signal(&count_threshold_cv);  // Signal the waiting thread
        }

        printf("inc_count(): Thread %d, count = %d.\n", my_id, count);
        pthread_mutex_unlock(&count_mutex);  // Unlock the mutex

        // Sleep for a while to simulate time-consuming work
        usleep(100000);
    }
    return NULL;
}

// Thread that waits for count to reach COUNT_LIMIT
void *watch_count(void *idp) {
    pthread_mutex_lock(&count_mutex);  // Lock the mutex

    // Wait until count reaches the predefined limit
    while (count < COUNT_LIMIT) {
        printf("watch_count(): Thread waiting. count = %d\n", count);
        pthread_cond_wait(&count_threshold_cv, &count_mutex);
    }

    printf("watch_count(): Thread signaled. count = %d\n", count);

    // Once awakened, modify the count
    count += 10;
    printf("watch_count(): count now = %d\n", count);

    pthread_mutex_unlock(&count_mutex);  // Unlock the mutex
    return NULL;
}

int main() {
    pthread_t threads[3];  // Three threads
    int thread_ids[3] = {1, 2, 3};  // Thread IDs

    // Initialize mutex and condition variable
    pthread_mutex_init(&count_mutex, NULL);
    pthread_cond_init(&count_threshold_cv, NULL);

    // Create the threads
    pthread_create(&threads[0], NULL, inc_count, (void *)&thread_ids[0]);
    pthread_create(&threads[1], NULL, inc_count, (void *)&thread_ids[1]);
    pthread_create(&threads[2], NULL, watch_count, (void *)&thread_ids[2]);

    // Wait for all threads to complete
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print final value of count
    printf("Main: Final count = %d\n", count);

    // Clean up and exit
    pthread_mutex_destroy(&count_mutex);
    pthread_cond_destroy(&count_threshold_cv);

    return 0;
}
