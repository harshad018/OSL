#include <stdio.h>

int main() {
  int arrival_time[10], burst_time[10], temp[10];
  int i, smallest, count = 0, time, limit;
  double wait_time = 0, turnaround_time = 0, end;
  float average_waiting_time, average_turnaround_time;

  printf("Enter the Total Number of Processes: ");
  scanf("%d", &limit);

  printf("Enter Details of %d Processes\n", limit);
  for (i = 0; i < limit; i++) {
    printf("Enter Arrival Time: ");
    scanf("%d", &arrival_time[i]);
    printf("Enter Burst Time: ");
    scanf("%d", &burst_time[i]);
    temp[i] = burst_time[i];
  }

  

  for (time = 0; count != limit; time++) {
    smallest = -1;  // Initialize smallest to -1 to indicate no process found

    for (i = 0; i < limit; i++) {
      // Check if the process has arrived and has remaining burst time
      if (arrival_time[i] <= time && burst_time[i] > 0) {
        // If it's the first process or has a smaller burst time than the current
        // smallest, update smallest
        if (smallest == -1 || burst_time[i] < burst_time[smallest]) {
          smallest = i;
        }
      }
    }

    // If a process was found with a smaller burst time
    if (smallest != -1) {
      burst_time[smallest]--;
      if (burst_time[smallest] == 0) {
        count++;
        end = time + 1;
        wait_time += end - arrival_time[smallest] - temp[smallest];
        turnaround_time += end - arrival_time[smallest];
      }
    }
  }

  average_waiting_time = wait_time / limit;
  average_turnaround_time = turnaround_time / limit;

  printf("\nAverage Waiting Time: %lf\n", average_waiting_time);
  printf("Average Turnaround Time: %lf\n", average_turnaround_time);

  return 0;
}