#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lab6b_1.h"
#include "lab6b_2.h"
#include "lab6b_3.h"

int main() {
    int choice;
    
    printf("Choose the version of program to run:\n");
    printf("1. Simple Linked List\n");
    printf("2. Double Linked List\n");
    printf("3. Circular Linked List\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            simpleLinkedListProgram();
            break;
        case 2:
            doubleLinkedListProgram();
            break;
        case 3:
            circularLinkedListProgram();
            break;
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}
