#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5
#define NUM_MEALS 3  // Number of meals each philosopher eats before finishing

pthread_mutex_t chopsticks[NUM_PHILOSOPHERS];

void* philosopher(void* num) {
    int id = *(int*)num;
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;
    int meals = 0;

    while (meals < NUM_MEALS) {
        printf("Philosopher %d is thinking.\n", id + 1);
        sleep(1);

        if (id % 2 == 0) {
            // Even-numbered philosopher picks left first
            pthread_mutex_lock(&chopsticks[left]);
            printf("Philosopher %d picked up left chopstick %d.\n", id + 1, left);

            pthread_mutex_lock(&chopsticks[right]);
            printf("Philosopher %d picked up right chopstick %d.\n", id + 1, right);
        } else {
            // Odd-numbered philosopher picks right first
            pthread_mutex_lock(&chopsticks[right]);
            printf("Philosopher %d picked up right chopstick %d.\n", id + 1, right);

            pthread_mutex_lock(&chopsticks[left]);
            printf("Philosopher %d picked up left chopstick %d.\n", id + 1, left);
        }

        printf("Philosopher %d is eating meal %d.\n", id + 1, meals + 1);
        sleep(1);
        meals++;

        pthread_mutex_unlock(&chopsticks[left]);
        pthread_mutex_unlock(&chopsticks[right]);
        printf("Philosopher %d put down both chopsticks.\n", id + 1);
    }

    printf("Philosopher %d is done eating.\n", id + 1);
    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize chopstick mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&chopsticks[i], NULL);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &philosopher_ids[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy chopstick mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&chopsticks[i]);
    }

    printf("All philosophers are done eating. Program ending.\n");
    return 0;
}
