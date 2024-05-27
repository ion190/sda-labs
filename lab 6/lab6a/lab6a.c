#include <stdio.h>
#include "lab6a_1.h"
#include "lab6a_2.h"

int main() {
    int choice;
    
    // Display menu
    printf("Choose the version of program to run:\n");
    printf("1. stackProgram 1\n");
    printf("2. stackProgram 2\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Execute the selected program based on the choice
    switch(choice) {
        case 1:
            stackProgram();
            break;
        case 2:
            Queue_queueProgram();
            break;
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}
