#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

// Structure to represent a process
typedef struct {
    int burst_time;
    int arrival_time;
} Process;

// Function to implement Round Robin scheduling
void round_robin(Process processes[], int n, int time_quantum) {
    int remaining_times[MAX_PROCESSES];
    int completion_times[MAX_PROCESSES];
    int waiting_times[MAX_PROCESSES];
    int turnaround_times[MAX_PROCESSES];

    // Initialize remaining times with burst times
    for (int i = 0; i < n; i++) {
        remaining_times[i] = processes[i].burst_time;
    }

    int queue[MAX_PROCESSES];
    int head = 0, tail = 0; // Circular queue implementation using an array
    int time = 0;
    int current_process = -1;

    while (1) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && remaining_times[i] > 0) {
                int in_queue = 0;  // Flag to check if process is already in queue
                for (int j = head; j != tail; j = (j + 1) % MAX_PROCESSES) {
                    if (queue[j] == i) {
                        in_queue = 1;
                        break;
                    }
                }
                if (!in_queue) {  // Add process to queue if it's not already there
                    queue[tail] = i;
                    tail = (tail + 1) % MAX_PROCESSES;
                }
            }
        }

        if (head == tail && current_process == -1) { // Queue is empty and no process is running
            break;
        }

        if (current_process != -1 && remaining_times[current_process] > 0) {
            int in_queue = 0;
            for (int j = head; j != tail; j = (j + 1) % MAX_PROCESSES) {
                if (queue[j] == current_process) {
                    in_queue = 1;
                    break;
                }
            }
            if (!in_queue) {
                queue[tail] = current_process;
                tail = (tail + 1) % MAX_PROCESSES;
            }
        } else if (current_process != -1 && remaining_times[current_process] == 0) {
            completion_times[current_process] = time;
            current_process = -1;
        }

        if (head != tail) {  // Queue not empty
            current_process = queue[head];
            head = (head + 1) % MAX_PROCESSES;


            int time_slice = (time_quantum < remaining_times[current_process]) ? time_quantum : remaining_times[current_process];
            remaining_times[current_process] -= time_slice;
            time += time_slice;

            if (remaining_times[current_process] == 0) {
                completion_times[current_process] = time;
                current_process = -1;
            }
        }



    }

    float avg_turnaround_time = 0;  // Use float
    float avg_waiting_time = 0;    // Use float

    for (int i = 0; i < n; i++) {
        turnaround_times[i] = completion_times[i] - processes[i].arrival_time;
        waiting_times[i] = turnaround_times[i] - processes[i].burst_time;
        avg_turnaround_time += turnaround_times[i];
        avg_waiting_time += waiting_times[i];
    }

    avg_turnaround_time /= (float)n;  // Correct float division
    avg_waiting_time /= (float)n;     // Correct float division

    printf("Average Turn around time = %.2f\n", avg_turnaround_time);
    printf("Average waiting time = %.2f\n", avg_waiting_time);
}



int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    Process processes[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        printf("Enter burst time for process P%d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
        printf("Enter arrival time for process P%d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
    }

    int time_quantum;
    printf("Enter time quantum: ");
    scanf("%d", &time_quantum);

    round_robin(processes, n, time_quantum);

    return 0;
}