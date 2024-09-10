#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // For sleep function

// Global variable for account balance
int account = 1000;

// Mutex and condition variable for synchronization
pthread_mutex_t account_mutex;
pthread_cond_t account_cond;

// Deposit function
void deposit(int amount) {
    pthread_mutex_lock(&account_mutex); // Lock the mutex
    account += amount; // Deposit the amount
    printf("Deposited: %d, Current Account Balance: %d\n", amount, account);

    // Signal any waiting withdrawal threads
    if (account > 1000) {
        pthread_cond_broadcast(&account_cond);
    }

    pthread_mutex_unlock(&account_mutex); // Unlock the mutex
}

// Withdrawal function
void withdrawal(int amount) {
    pthread_mutex_lock(&account_mutex); // Lock the mutex

    // Block the thread if the account balance is less than 1000
    while (account < 1000) {
        printf("Insufficient balance for withdrawal. Current Account Balance: %d. Waiting...\n", account);
        pthread_cond_wait(&account_cond, &account_mutex); // Wait for condition signal
    }

    // Withdraw the amount
    account -= amount;
    printf("Withdrawn: %d, Current Account Balance: %d\n", amount, account);

    pthread_mutex_unlock(&account_mutex); // Unlock the mutex
}

// Thread function for depositing money
void* deposit_thread(void* arg) {
    for (int i = 0; i < 5; i++) {
        deposit(200); // Deposit 200 each time
        sleep(1); // Sleep for 1 second to simulate processing time
    }
    pthread_exit(NULL); // Terminate the thread
}

// Thread function for withdrawing money
void* withdrawal_thread(void* arg) {
    for (int i = 0; i < 5; i++) {
        withdrawal(100); // Withdraw 100 each time
        sleep(2); // Sleep for 2 seconds to simulate processing time
    }
    pthread_exit(NULL); // Terminate the thread
}

int main() {
    pthread_t deposit_tid, withdrawal_tid; // Thread identifiers

    // Initialize the mutex and condition variable
    pthread_mutex_init(&account_mutex, NULL);
    pthread_cond_init(&account_cond, NULL);

    // Create threads
    pthread_create(&deposit_tid, NULL, deposit_thread, NULL);
    pthread_create(&withdrawal_tid, NULL, withdrawal_thread, NULL);

    // Wait for both threads to complete
    pthread_join(deposit_tid, NULL);
    pthread_join(withdrawal_tid, NULL);

    // Destroy the mutex and condition variable
    pthread_mutex_destroy(&account_mutex);
    pthread_cond_destroy(&account_cond);

    printf("Final Account Balance: %d\n", account);
    return 0;
}