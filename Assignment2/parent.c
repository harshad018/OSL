#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_NUMBERS 10 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <child_executable>\n", argv[0]);
        exit(1);
    }

    int numbers[MAX_NUMBERS];
    int count = 0;

    // userinput
    printf("Enter the number of integers to sort (max %d): ", MAX_NUMBERS);
    if (scanf("%d", &count) != 1 || count <= 0 || count > MAX_NUMBERS) {
        fprintf(stderr, "Invalid input. Please enter a positive integer between 1 and %d.\n", MAX_NUMBERS);
        exit(1);
    }

    
    printf("Enter %d integers: ", count);
    for (int i = 0; i < count; i++) {
        if (scanf("%d", &numbers[i]) != 1) {
            fprintf(stderr, "Invalid input.\n");
            exit(1);
        }
    }

    //part a: zombie, fork 
    pid_t child_pid = fork();

    if (child_pid == 0) { // Child Process
        // Child sorts the numbers 
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (numbers[i] > numbers[j]) {
                    int temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }
            }
        }

        //zombie child
        exit(0);
    } else if (child_pid > 0) { 
        
        wait(NULL);

        //sorting
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (numbers[i] > numbers[j]) {
                    int temp = numbers[i];
                    numbers[i] = numbers[j];
                    numbers[j] = temp;
                }
            }
        }

        //parent output
        printf("Parent process sorted array: ");
        for (int i = 0; i < count; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    } else {
        perror("Fork failed");
        exit(1);
    }

    // execv commnad
    child_pid = fork();

    if (child_pid == 0) { 
       
        char *child_args[count + 2];
        child_args[0] = argv[1]; // child filename
        for (int i = 0; i < count; i++) {
            child_args[i + 1] = malloc(sizeof(char) * 10);
            sprintf(child_args[i + 1], "%d", numbers[i]); 
        }
        child_args[count + 1] = NULL; 

       
        execv(argv[1], child_args);
        perror("execv failed");
        exit(1);
    } else if (child_pid > 0) { 
        
        wait(NULL); // 
        printf("Parent process completed.\n");
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}

