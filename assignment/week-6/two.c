#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define QUEUE_SIZE 5  // Define the size of the queue

char queue[QUEUE_SIZE];
int front = 0, rear = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

// Function for the producer thread
void* producer(void* arg) {
    FILE* file = fopen("string.txt", "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        pthread_mutex_lock(&mutex);

        // Wait until there is space in the queue
        while (count == QUEUE_SIZE) {
            pthread_cond_wait(&not_full, &mutex);
        }

        // Add the character to the queue
        queue[rear] = ch;
        rear = (rear + 1) % QUEUE_SIZE;
        count++;

        pthread_cond_signal(&not_empty);  // Signal that the queue is not empty
        pthread_mutex_unlock(&mutex);
    }

    // Signal the end of the producer
    pthread_mutex_lock(&mutex);
    queue[rear] = '\0';  // End of string character
    rear = (rear + 1) % QUEUE_SIZE;
    count++;
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&mutex);

    fclose(file);
    return NULL;
}

// Function for the consumer thread
void* consumer(void* arg) {
    char ch;
    while (1) {
        pthread_mutex_lock(&mutex);

        // Wait until there is data in the queue
        while (count == 0) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Read the character from the queue
        ch = queue[front];
        front = (front + 1) % QUEUE_SIZE;
        count--;

        pthread_cond_signal(&not_full);  // Signal that the queue is not full
        pthread_mutex_unlock(&mutex);

        if (ch == '\0') break;  // End of string detected
        putchar(ch);  // Print the character
        fflush(stdout);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("\nProgram completed.\n");
    return 0;
}
