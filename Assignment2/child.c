#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("Child process received array in reverse order: ");
    for (int i = argc - 1; i > 0; i--) { // Loop in reverse
        printf("%d ", atoi(argv[i]));
    }
    printf("\n");
    return 0;
}

