#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0;  // Track the number of items in the buffer

void *producer(void *arg) {
    int i = 0;
    while (1) {
        if (count < BUFFER_SIZE) {  // Check if the buffer is not full
            buffer[count] = i;      // Add an item to the buffer
            printf("Producer produced: %d, Buffer count: %d\n", i, count + 1);
            count++;                // Increment buffer count
            i++;
        } else {
            printf("Buffer full! Producer waiting...\n");
        }
        sleep(rand() % 3);  // Simulate time for producing
    }
}

void *consumer(void *arg) {
    while (1) {
        if (count > 0) {  // Check if the buffer is not empty
            int item = buffer[count - 1];  // Remove an item from the buffer
            printf("Consumer consumed: %d, Buffer count: %d\n", item, count - 1);
            count--;  // Decrement buffer count
        } else {
            printf("Buffer empty! Consumer waiting...\n");
        }
        sleep(rand() % 3);  // Simulate time for consuming
    }
}

int main() {
    pthread_t prod, cons;

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Join the threads (though they will run indefinitely in this example)
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
