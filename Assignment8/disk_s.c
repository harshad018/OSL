#include <stdio.h>
#include <stdlib.h>

int RQ[100], i, n, TotalHeadMoment = 0, initial, count = 0;
int size, move; // Global variables for request queue, head position, total head movement, disk size, and movement direction

// Function to get user input for requests and initial head position
void getData() {
    printf("Enter the number of Requests: ");
    scanf("%d", &n); // Number of requests
    printf("Enter the Requests sequence: ");
    for (i = 0; i < n; i++)
        scanf("%d", &RQ[i]); // Request sequence
    printf("Enter initial head position: ");
    scanf("%d", &initial); // Initial position of the disk head
}

// Function to implement Shortest Seek Time First (SSTF) scheduling
void sstf() {
    TotalHeadMoment = 0;  // Reset total head movement
    int served[100] = {0}; // Array to track which requests have been served

    while (count < n) { // Process all requests
        int min = 1000, index = -1; // Initialize minimum distance and index
        for (i = 0; i < n; i++) {
            if (!served[i]) { // Only consider unserved requests
                int d = abs(RQ[i] - initial); // Calculate distance to each request
                if (min > d) { // Find the closest request
                    min = d;
                    index = i; // Update index of the closest request
                }
            }
        }
        if (index != -1) { // If a valid request is found
            TotalHeadMoment += min; // Update total head movement
            initial = RQ[index]; // Move to the requested position
            served[index] = 1; // Mark this request as served
            count++;
        }
    }

    printf("Total head movement is %d\n", TotalHeadMoment); // Output total movement
}

// Function to sort the request array using Bubble Sort
void sortRequests() {
    for (i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (RQ[j] > RQ[j + 1]) { // Swap if elements are out of order
                int temp = RQ[j];
                RQ[j] = RQ[j + 1];
                RQ[j + 1] = temp;
            }
        }
    }
}

// Function to implement SCAN scheduling
void scan() {
    TotalHeadMoment = 0;  // Reset total head movement
    printf("Enter total disk size: ");
    scanf("%d", &size); // Total size of the disk
    printf("Enter the head movement direction for high (1) and for low (0): ");
    scanf("%d", &move); // Direction of movement

    sortRequests(); // Sort the request sequence

    int index; // To find the starting point in the request array
    for (i = 0; i < n; i++) {
        if (initial < RQ[i]) { // Find the first request that is greater than the initial position
            index = i;
            break;
        }
    }

    if (move == 1) { // If moving towards high values
        for (i = index; i < n; i++) { // Serve all requests from the index to the end
            TotalHeadMoment += abs(RQ[i] - initial); // Calculate distance
            initial = RQ[i]; // Move the head
        }
        TotalHeadMoment += abs(size - RQ[n - 1] - 1); // Move to the end of the disk
        initial = size - 1; // Update head position to the end
        for (i = index - 1; i >= 0; i--) { // Serve remaining requests in low direction
            TotalHeadMoment += abs(RQ[i] - initial);
            initial = RQ[i];
        }
    } else { // If moving towards low values
        for (i = index - 1; i >= 0; i--) { // Serve all requests from the index to the start
            TotalHeadMoment += abs(RQ[i] - initial);
            initial = RQ[i];
        }
        TotalHeadMoment += abs(RQ[0] - 0); // Move to the beginning of the disk
        initial = 0; // Update head position to the start
        for (i = index; i < n; i++) { // Serve remaining requests in high direction
            TotalHeadMoment += abs(RQ[i] - initial);
            initial = RQ[i];
        }
    }

    printf("Total head movement is %d\n", TotalHeadMoment); // Output total movement
}

// Function to implement C-LOOK scheduling
void clook() {
    TotalHeadMoment = 0;  // Reset total head movement
    printf("Enter total disk size: ");
    scanf("%d", &size); // Total size of the disk
    printf("Enter the head movement direction for high (1) and for low (0): ");
    scanf("%d", &move); // Direction of movement

    sortRequests(); // Sort the request sequence

    int index; // To find the starting point in the request array
    for (i = 0; i < n; i++) {
        if (initial < RQ[i]) { // Find the first request that is greater than the initial position
            index = i;
            break;
        }
    }

    if (move == 1) { // If moving towards high values
        for (i = index; i < n; i++) { // Serve all requests from the index to the end
            TotalHeadMoment += abs(RQ[i] - initial);
            initial = RQ[i];
        }
        if (index > 0) { // If there are requests before the index
            TotalHeadMoment += abs(RQ[0] - initial); // Move to the first request
            initial = RQ[0]; // Update head position
        }
    } else { // If moving towards low values
        for (i = index - 1; i >= 0; i--) { // Serve all requests from the index to the start
            TotalHeadMoment += abs(RQ[i] - initial);
            initial = RQ[i];
        }
        if (index < n) { // If there are requests after the index
            TotalHeadMoment += abs(RQ[n - 1] - initial); // Move to the last request
            initial = RQ[n - 1]; // Update head position
        }
    }

    printf("Total head movement is %d\n", TotalHeadMoment); // Output total movement
}

// Main function to drive the program
int main() {
    int choice; // Variable to store user's menu choice
    while (1) {
        // Display the menu
        printf("\nDisk Scheduling Algorithms\n1. Enter data\n2. SSTF\n3. SCAN\n4. C-LOOK\n5. Exit\nEnter your choice: ");
        scanf("%d", &choice); // Get user's choice
        switch (choice) {
            case 1:
                getData(); // Get input data
                break;
            case 2:
                sstf(); // Call SSTF algorithm
                break;
            case 3:
                scan(); // Call SCAN algorithm
                break;
            case 4:
                clook(); // Call C-LOOK algorithm
                break;
            default:
                return 0; // Exit the program
        }
    }
}
