#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define READERS_COUNT 5
#define WRITERS_COUNT 2

// Shared data
int shared_data = 0;
int reader_count = 0;

// Mutexes and condition variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reader_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex for reader count
pthread_cond_t can_write = PTHREAD_COND_INITIALIZER;   // Condition variable for writers

// Reader thread function
void *reader(void *arg) {
    int reader_id = *(int *)arg;

    while (1) {
        // Entry section for readers
        pthread_mutex_lock(&reader_mutex); // Lock reader_mutex to protect reader_count
        reader_count++;
        if (reader_count == 1) { // If first reader, acquire main mutex to exclude writers
            pthread_mutex_lock(&mutex);
        }
        pthread_mutex_unlock(&reader_mutex); // Release reader_mutex


        // Critical section (reading)
        printf("Reader %d is reading: %d\n", reader_id, shared_data);
        usleep(100000); // Simulate reading delay

        // Exit section for readers
        pthread_mutex_lock(&reader_mutex); // Lock reader_mutex to protect reader_count
        reader_count--;
        if (reader_count == 0) {  // If last reader, release main mutex allowing writers
            pthread_mutex_unlock(&mutex);
            pthread_cond_signal(&can_write); // Signal a waiting writer (if any)
        }
        pthread_mutex_unlock(&reader_mutex); // Release reader_mutex
        
        usleep(200000); // Simulate some non-critical section time for readers
    }
    return NULL;
}

// Writer thread function
void *writer(void *arg) {
    int writer_id = *(int *)arg;

    while (1) {
        // Entry section for writers (acquire both mutexes)
        pthread_mutex_lock(&mutex); // Lock the main mutex to block both readers & other writers

        while (reader_count > 0) { // Check if any readers are active
            pthread_cond_wait(&can_write, &mutex); // Wait until all readers finish
        }

        // Critical section (writing)
        shared_data++;
        printf("Writer %d is writing: %d\n", writer_id, shared_data);
        usleep(150000); // Simulate writing delay


        // Exit section for writers (release the main mutex)
        pthread_mutex_unlock(&mutex);

        usleep(250000); // Simulate some non-critical section time for writers
    }
    return NULL;
}

int main() {
    pthread_t readers[READERS_COUNT];
    pthread_t writers[WRITERS_COUNT];

    int reader_ids[READERS_COUNT];
    int writer_ids[WRITERS_COUNT];

    // Create reader threads
    for (int i = 0; i < READERS_COUNT; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < WRITERS_COUNT; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Join threads (keep main thread running indefinitely)
    for (int i = 0; i < READERS_COUNT; i++) {
        pthread_join(readers[i], NULL); // This will never actually happen in this example.
    }
    for (int i = 0; i < WRITERS_COUNT; i++) {
        pthread_join(writers[i], NULL); // This will never actually happen either.
    }


    return 0;
}