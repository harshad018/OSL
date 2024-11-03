#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10

// Shared buffer and variables
int buffer[BUFFER_SIZE];
int count = 0;  // Number of items in buffer
int in = 0;     // Index for adding items
int out = 0;    // Index for removing items

// Synchronization objects
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

// Producer function
void* producer(void* arg) {
    while (1) {
        int item = rand() % 100;  // Generate random item

        pthread_mutex_lock(&mutex);
        
        // Wait if buffer is full
        while (count == BUFFER_SIZE) {
            printf("Buffer full, producer waiting...\n");
            pthread_cond_wait(&empty, &mutex);
        }
        
        // Add item to buffer
        buffer[in] = item;
        printf("Producer produced item %d at position %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        // Signal consumer that buffer is no longer empty
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
        
        sleep(1);  // Simulate some work
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        
        // Wait if buffer is empty
        while (count == 0) {
            printf("Buffer empty, consumer waiting...\n");
            pthread_cond_wait(&full, &mutex);
        }
        
        // Remove item from buffer
        int item = buffer[out];
        printf("Consumer consumed item %d from position %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        
        // Signal producer that buffer is no longer full
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        
        sleep(2);  // Simulate some work
    }
    return NULL;
}

int main() {
    // Thread handles
    pthread_t producer_thread, consumer_thread;
    
    // Create threads
    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        perror("Producer thread creation failed");
        return 1;
    }
    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        perror("Consumer thread creation failed");
        return 1;
    }
    
    // Wait for threads to finish (they won't in this case)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    return 0;
}