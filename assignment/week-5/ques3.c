#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep function

// Global variable for balance
int balance = 1000;

// Mutex for synchronizing access to the balance
pthread_mutex_t balance_mutex;

// Function for depositing money
void* deposit(void* arg) {
    for (int i = 0; i < 50; i++) {
        pthread_mutex_lock(&balance_mutex); // Lock the mutex

        balance += 50; // Deposit amount
        printf("Deposited: 50, Current Balance: %d\n", balance);

        pthread_mutex_unlock(&balance_mutex); // Unlock the mutex
        usleep(100000); // Sleep for 100 milliseconds to simulate processing time
    }
    pthread_exit(NULL); // Terminate the thread
}

// Function for withdrawing money
void* withdraw(void* arg) {
    for (int i = 0; i < 20; i++) {
        pthread_mutex_lock(&balance_mutex); // Lock the mutex

        if (balance >= 20) {
            balance -= 20; // Withdraw amount
            printf("Withdrawn: 20, Current Balance: %d\n", balance);
        } else {
            printf("Insufficient balance for withdrawal. Current Balance: %d\n", balance);
        }

        pthread_mutex_unlock(&balance_mutex); // Unlock the mutex
        usleep(150000); // Sleep for 150 milliseconds to simulate processing time
    }
    pthread_exit(NULL); // Terminate the thread
}

int main() {
    pthread_t deposit_thread, withdraw_thread; // Thread identifiers

    // Initialize the mutex
    pthread_mutex_init(&balance_mutex, NULL);

    // Create threads
    pthread_create(&deposit_thread, NULL, deposit, NULL);
    pthread_create(&withdraw_thread, NULL, withdraw, NULL);

    // Wait for both threads to complete
    pthread_join(deposit_thread, NULL);
    pthread_join(withdraw_thread, NULL);

    // Destroy the mutex
    pthread_mutex_destroy(&balance_mutex);

    printf("Final Balance: %d\n", balance);
    return 0;
}