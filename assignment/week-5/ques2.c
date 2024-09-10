#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Structure to hold thread information
typedef struct {
    const char* name;
    const char* hall_no;
    const char* employee_id;
    const char* branch;
} ThreadInfo;

// Function that each thread will execute
void* print_info(void* arg) {
    ThreadInfo* info = (ThreadInfo*)arg; // Cast argument to ThreadInfo pointer

    printf("Name: %s\n", info->name);
    printf("Hall No: %s\n", info->hall_no);
    printf("Employee ID: %s\n", info->employee_id);
    printf("Branch: %s\n", info->branch);
    printf("-------------------------\n");

    pthread_exit(NULL); // Terminate the thread
}

int main() {
    pthread_t threads[2]; // Array to hold thread identifiers
    ThreadInfo thread_info[2]; // Array to hold information for each thread

    // Initialize information for Thread 1
    thread_info[0].name = "Alice Smith";
    thread_info[0].hall_no = "Hall A";
    thread_info[0].employee_id = "E12345";
    thread_info[0].branch = "Computer Science";

    // Initialize information for Thread 2
    thread_info[1].name = "Bob Johnson";
    thread_info[1].hall_no = "Hall B";
    thread_info[1].employee_id = "E67890";
    thread_info[1].branch = "Electrical Engineering";

    // Create 2 threads
    for (int i = 0; i < 2; i++) {
        int rc = pthread_create(&threads[i], NULL, print_info, (void*)&thread_info[i]);
        if (rc) {
            fprintf(stderr, "Error: Unable to create thread %d, %d\n", i, rc);
            exit(-1);
        }
    }

    // Wait for both threads to complete
    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("All threads completed successfully.\n");
    return 0;
}