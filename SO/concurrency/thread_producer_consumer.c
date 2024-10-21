#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NUM 10

pthread_mutex_t mutex;
pthread_cond_t cond_var;

int shared_data = 0;
int data_available = 0; // Flag to indicate if data is available

// Producer thread function
void* producer(void* arg) {
    for (int i = 0; i < MAX_NUM; i++) {
        // Simulate some work
        sleep(1);

        // Generate random number
        int num = rand() % 100;

        // Lock the mutex to access shared data
        pthread_mutex_lock(&mutex);
        
        // Update shared data
        shared_data = num;
        data_available = 1; // Mark data as available

        // Signal the consumer that data is available
        pthread_cond_signal(&cond_var);

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

// Consumer thread function
void* consumer(void* arg) {
    for (int i = 0; i < MAX_NUM; i++) {
        // Lock the mutex to access shared data
        pthread_mutex_lock(&mutex);

        // Wait for data from producer
        while (!data_available) {
            pthread_cond_wait(&cond_var, &mutex);
        }

        // Consume the data (print it)
        printf("Consumed: %d\n", shared_data);
        fflush(stdout);

        // Mark data as consumed
        data_available = 0;

        // Unlock the mutex
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Initialize mutex and condition variable
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}
