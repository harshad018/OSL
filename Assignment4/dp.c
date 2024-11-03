#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>

#define NUM_PHILOSOPHERS 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define MAX_MEALS 3  // Maximum number of meals per philosopher

// Global variables
sem_t chopsticks[NUM_PHILOSOPHERS];
pthread_mutex_t mutex;
int state[NUM_PHILOSOPHERS];
int meals_eaten[NUM_PHILOSOPHERS] = {0};
volatile sig_atomic_t program_running = 1;

// Statistics
int total_wait_time[NUM_PHILOSOPHERS] = {0};
time_t last_state_change[NUM_PHILOSOPHERS];

void signal_handler(int signum) {
    program_running = 0;
}

void print_state(int phil_id, const char* action) {
    static const char* state_names[] = {"THINKING", "HUNGRY", "EATING"};
    time_t now = time(NULL);
    
    printf("Philosopher %d: %s (State: %s, Meals: %d, Wait Time: %ds)\n",
           phil_id, action, state_names[state[phil_id]], 
           meals_eaten[phil_id], total_wait_time[phil_id]);
}

void test(int phil_id) {
    // Check if philosopher can eat (both neighbors are not eating)
    if (state[phil_id] == HUNGRY &&
        state[(phil_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS] != EATING &&
        state[(phil_id + 1) % NUM_PHILOSOPHERS] != EATING) {
        
        state[phil_id] = EATING;
        time_t wait_time = time(NULL) - last_state_change[phil_id];
        total_wait_time[phil_id] += (int)wait_time;
        
        print_state(phil_id, "starts eating");
        
        // Signal that philosopher can eat
        sem_post(&chopsticks[phil_id]);
    }
}

void take_chopsticks(int phil_id) {
    pthread_mutex_lock(&mutex);
    
    state[phil_id] = HUNGRY;
    last_state_change[phil_id] = time(NULL);
    print_state(phil_id, "is hungry");
    
    test(phil_id);
    pthread_mutex_unlock(&mutex);
    
    // Wait if chopsticks are not available
    sem_wait(&chopsticks[phil_id]);
}

void put_chopsticks(int phil_id) {
    pthread_mutex_lock(&mutex);
    
    state[phil_id] = THINKING;
    meals_eaten[phil_id]++;
    print_state(phil_id, "finished eating");
    
    // Let neighbors eat if they're hungry
    test((phil_id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS);
    test((phil_id + 1) % NUM_PHILOSOPHERS);
    
    pthread_mutex_unlock(&mutex);
}

void* philosopher(void* arg) {
    int phil_id = *(int*)arg;
    
    while (program_running && meals_eaten[phil_id] < MAX_MEALS) {
        // Thinking
        print_state(phil_id, "is thinking");
        sleep(rand() % 3 + 1);
        
        if (!program_running) break;
        
        // Get hungry and try to eat
        take_chopsticks(phil_id);
        
        if (!program_running) {
            state[phil_id] = THINKING;
            break;
        }
        
        // Eating
        sleep(rand() % 2 + 1);
        
        // Finished eating
        put_chopsticks(phil_id);
    }
    
    printf("Philosopher %d finished dining. Total meals: %d\n", 
           phil_id, meals_eaten[phil_id]);
    return NULL;
}

void print_statistics() {
    printf("\nDining Session Statistics:\n");
    printf("-------------------------\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("Philosopher %d:\n", i);
        printf("  Meals eaten: %d\n", meals_eaten[i]);
        printf("  Total wait time: %d seconds\n", total_wait_time[i]);
        printf("  Average wait time per meal: %.2f seconds\n", 
               meals_eaten[i] > 0 ? (float)total_wait_time[i] / meals_eaten[i] : 0);
        printf("\n");
    }
}

int main() {
    // Set up signal handler
    signal(SIGINT, signal_handler);
    
    // Initialize random seed
    srand(time(NULL));
    
    // Initialize mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Mutex initialization failed\n");
        return 1;
    }
    
    // Initialize semaphores and states
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (sem_init(&chopsticks[i], 0, 0) != 0) {
            printf("Semaphore initialization failed\n");
            return 1;
        }
        state[i] = THINKING;
        last_state_change[i] = time(NULL);
    }
    
    // Create philosopher threads
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int phil_ids[NUM_PHILOSOPHERS];
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        phil_ids[i] = i;
        if (pthread_create(&philosophers[i], NULL, philosopher, &phil_ids[i]) != 0) {
            printf("Thread creation failed\n");
            return 1;
        }
        printf("Philosopher %d joined the table\n", i);
        usleep(100000);  // Small delay between thread creation
    }
    
    // Wait for philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }
    
    // Print final statistics
    print_statistics();
    
    // Cleanup
    pthread_mutex_destroy(&mutex);
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }
    
    return 0;
}