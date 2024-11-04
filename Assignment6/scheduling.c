#include<stdio.h>

int n, nf; // Number of pages, number of frames
int in[100]; // Page reference sequence
int p[50]; // Frame array to hold pages
int hit = 0; // Hit flag
int i, j, k; // Loop counters
int pgfaultcnt = 0; // Page fault count

// Function to get user input
void getData() {
    printf("\nEnter length of page reference sequence:");
    scanf("%d", &n); // Input number of pages
    printf("\nEnter the page reference sequence:");
    for (i = 0; i < n; i++)
        scanf("%d", &in[i]); // Input page reference sequence
    printf("\nEnter no of frames:");
    scanf("%d", &nf); // Input number of frames
}

// Initialize frame array and page fault counter
void initialize() {
    pgfaultcnt = 0; // Reset page fault count
    for (i = 0; i < nf; i++)
        p[i] = 9999; // Initialize frames to an invalid value
}

// Check if a page is already in a frame
int isHit(int data) {
    hit = 0;
    for (j = 0; j < nf; j++) {
        if (p[j] == data) {
            hit = 1; // Page hit
            break;
        }
    }
    return hit; // Return hit status
}

// Display current pages in frames
void dispPages() {
    for (k = 0; k < nf; k++) {
        if (p[k] != 9999)
            printf(" %d", p[k]); // Print current pages in frames
    }
}

// Display total number of page faults
void dispPgFaultCnt() {
    printf("\nTotal no of page faults:%d", pgfaultcnt);
}

// FIFO Page Replacement Algorithm
void fifo() {
    initialize(); // Initialize frames and page fault counter
    for (i = 0; i < n; i++) {
        printf("\nFor %d :", in[i]); // Current page being processed

        if (isHit(in[i]) == 0) { // If page is not in frames
            for (k = 0; k < nf - 1; k++)
                p[k] = p[k + 1]; // Shift pages to the left

            p[k] = in[i]; // Add new page to the end
            pgfaultcnt++; // Increment page fault counter
            dispPages(); // Display current frame status
        } else
            printf("No page fault"); // No fault occurred
    }
    dispPgFaultCnt(); // Display total page faults
}

// Optimal Page Replacement Algorithm
void optimal() {
    initialize(); // Initialize frames and page fault counter
    int near[50]; // Array to hold distances for optimal replacement
    for (i = 0; i < n; i++) {
        printf("\nFor %d :", in[i]); // Current page being processed

        if (isHit(in[i]) == 0) { // If page is not in frames
            // Determine when each page will be used next
            for (j = 0; j < nf; j++) {
                int pg = p[j];
                int found = 0;
                for (k = i; k < n; k++) {
                    if (pg == in[k]) {
                        near[j] = k; // Record next use position
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    near[j] = 9999; // If not found, set to max
            }

            // Find the page to replace
            int max = -9999;
            int repindex; // Index of page to replace
            for (j = 0; j < nf; j++) {
                if (near[j] > max) {
                    max = near[j]; // Find furthest page
                    repindex = j; // Save index of that page
                }
            }
            p[repindex] = in[i]; // Replace page
            pgfaultcnt++; // Increment page fault counter

            dispPages(); // Display current frame status
        } else
            printf("No page fault"); // No fault occurred
    }
    dispPgFaultCnt(); // Display total page faults
}

// LRU Page Replacement Algorithm
void lru() {
    initialize(); // Initialize frames and page fault counter
    int least[50]; // Array to hold last used positions
    for (i = 0; i < n; i++) {
        printf("\nFor %d :", in[i]); // Current page being processed

        if (isHit(in[i]) == 0) { // If page is not in frames
            // Determine when each page was last used
            for (j = 0; j < nf; j++) {
                int pg = p[j];
                int found = 0;
                for (k = i - 1; k >= 0; k--) {
                    if (pg == in[k]) {
                        least[j] = k; // Record last used position
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    least[j] = -9999; // If not found, set to min
            }

            // Find the page to replace
            int min = 9999;
            int repindex; // Index of page to replace
            for (j = 0; j < nf; j++) {
                if (least[j] < min) {
                    min = least[j]; // Find least recently used page
                    repindex = j; // Save index of that page
                }
            }
            p[repindex] = in[i]; // Replace page
            pgfaultcnt++; // Increment page fault counter

            dispPages(); // Display current frame status
        } else
            printf("No page fault!"); // No fault occurred
    }
    dispPgFaultCnt(); // Display total page faults
}

// Main function
int main() {
    int choice; // User choice for operations
    while (1) {
        printf("\nPage Replacement Algorithms\n1.Enter data\n2.FIFO\n3.Optimal\n4.LRU\n\n5.Exit\nEnter your choice:");
        scanf("%d", &choice); // Get user choice
        switch (choice) {
            case 1:
                getData(); // Enter data
                break;
            case 2:
                fifo(); // Perform FIFO
                break;
            case 3:
                optimal(); // Perform Optimal
                break;
            case 4:
                lru(); // Perform LRU
                break;
            default:
                return 0; // Exit program
        }
    }
}
