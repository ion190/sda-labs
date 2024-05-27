#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct CircularWarehouseRecord {
    char owner_name[50];
    char owner_surname[50];
    char product_name[50];
    char manufacturer[50];
    int contract_date_day;
    int contract_date_month;
    int contract_date_year;
    float wholesale_price;
    float unit_price;
    int quantity;
    int remaining_years;
    int remaining_months;
    int remaining_days;
};

// Node structure for the circular linked list
struct CircularNode {
    struct CircularWarehouseRecord data;
    struct CircularNode *next;
};

// Function prototypes
struct CircularNode *createCircularNode(struct CircularWarehouseRecord data);
void insertCircularNode(struct CircularNode **head, int position, struct CircularWarehouseRecord data);
void deleteCircularNode(struct CircularNode **head, int position);
void displayCircularList(struct CircularNode *head);
void destroyCircularList(struct CircularNode **head);
void saveCircularListToFile(struct CircularNode *head, const char *filename);
void readCircularListFromFile(struct CircularNode **head, const char *filename);
char *readCircularFilename();

void circularLinkedListProgram() {
    struct CircularNode *head = NULL;

    int num_records;
    printf("Enter the number of records to enter: ");
    scanf("%d", &num_records);

    // Input warehouse registry data and create the circular linked list
    for (int i = 0; i < num_records; i++) {
        struct CircularWarehouseRecord record;
        printf("Record %d:\n", i + 1);
        printf("Enter owner's name: ");
        scanf("%s", record.owner_name);
        printf("Enter owner's surname: ");
        scanf("%s", record.owner_surname);
        printf("Enter product name: ");
        scanf("%s", record.product_name);
        printf("Enter manufacturer: ");
        scanf("%s", record.manufacturer);
        printf("Enter contract conclusion date (day month year): ");
        scanf("%d %d %d", &record.contract_date_day, &record.contract_date_month, &record.contract_date_year);
        printf("Enter wholesale price: ");
        scanf("%f", &record.wholesale_price);
        printf("Enter unit price: ");
        scanf("%f", &record.unit_price);
        printf("Enter quantity (units): ");
        scanf("%d", &record.quantity);

        insertCircularNode(&head, i, record);
    }

    int choice;
    do {
        printf("\nCircular Warehouse Management System Menu:\n");
        printf("1. Display Warehouse Records\n");
        printf("2. Save Warehouse Records to File\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nWarehouse Records:\n");
                displayCircularList(head);
                break;
            case 2: {
                char *filename = readCircularFilename();
                saveCircularListToFile(head, filename);
                printf("Warehouse records saved to file '%s'.\n", filename);
                free(filename);
                break;
            }
            case 3:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 3.\n");
        }
    } while (choice != 3);

    // Destroy the circular linked list
    destroyCircularList(&head);
}

// Function to create a new node
struct CircularNode *createCircularNode(struct CircularWarehouseRecord data) {
    struct CircularNode *newNode = (struct CircularNode *)malloc(sizeof(struct CircularNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at a specified position
void insertCircularNode(struct CircularNode **head, int position, struct CircularWarehouseRecord data) {
    if (position < 0) {
        printf("Invalid position.\n");
        return;
    }

    struct CircularNode *newNode = createCircularNode(data);

    if (*head == NULL) {
        *head = newNode;
        (*head)->next = *head; // Circular link
        return;
    }

    struct CircularNode *current = *head;
    int count = 0;
    while (count < position - 1 && current->next != *head) {
        current = current->next;
        count++;
    }

    newNode->next = current->next;
    current->next = newNode;
    if (position == 0) {
        *head = newNode;
    }

    // Update the last node to point to the head for circularity
    struct CircularNode *last = *head;
    while (last->next != *head) {
        last = last->next;
    }
    last->next = *head;
}

// Function to delete a node at a specified position
void deleteCircularNode(struct CircularNode **head, int position) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }

    struct CircularNode *temp = *head;
    struct CircularNode *prev = NULL;
    int count = 0;

    // Find the node to be deleted
    while (temp->next != *head && count != position) {
        prev = temp;
        temp = temp->next;
        count++;
    }

    // If the first node is to be deleted
    if (position == 0) {
        struct CircularNode *last = *head;
        while (last->next != *head) {
            last = last->next;
        }
        last->next = temp->next;
        *head = temp->next;
    }

    // If the last node is to be deleted
    else if (temp->next == *head) {
        prev->next = *head;
    }

    // If a middle node is to be deleted
    else {
        prev->next = temp->next;
    }

    free(temp);
}

// Function to display the circular linked list
void displayCircularList(struct CircularNode *head) {
    if (head == NULL) {
        printf("List is empty.\n");
        return;
    }

    struct CircularNode *current = head;
    do {
        printf("Owner: %s %s\n", current->data.owner_name, current->data.owner_surname);
        printf("Product Name: %s\n", current->data.product_name);
        printf("Manufacturer: %s\n", current->data.manufacturer);
        printf("Contract Date: %d-%02d-%02d\n", current->data.contract_date_year, current->data.contract_date_month, current->data.contract_date_day);
        printf("Wholesale Price: %.2f\n", current->data.wholesale_price);
        printf("Unit Price: %.2f\n", current->data.unit_price);
        printf("Quantity: %d\n\n", current->data.quantity);
        current = current->next;
    } while (current != head);
}

// Function to destroy the circular linked list and free memory
void destroyCircularList(struct CircularNode **head) {
    if (*head == NULL) {
        return;
    }

    struct CircularNode *current = *head;
    struct CircularNode *temp;

    do {
        temp = current->next;
        free(current);
        current = temp;
    } while (current != *head);

    *head = NULL;
}

// Function to save the circular linked list to a file
void saveCircularListToFile(struct CircularNode *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct CircularNode *current = head;
    do {
        fprintf(file, "Owner: %s %s\n", current->data.owner_name, current->data.owner_surname);
        fprintf(file, "Product Name: %s\n", current->data.product_name);
        fprintf(file, "Manufacturer: %s\n", current->data.manufacturer);
        fprintf(file, "Contract Date: %d-%02d-%02d\n", current->data.contract_date_year, current->data.contract_date_month, current->data.contract_date_day);
        fprintf(file, "Wholesale Price: %.2f\n", current->data.wholesale_price);
        fprintf(file, "Unit Price: %.2f\n", current->data.unit_price);
        fprintf(file, "Quantity: %d\n\n", current->data.quantity);
        current = current->next;
    } while (current != head);

    fclose(file);
}

// Function to read a filename from the user
char *readCircularFilename() {
    char *filename = (char *)malloc(256 * sizeof(char));
    if (filename == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter filename: ");
    scanf("%s", filename);

    return filename;
}
