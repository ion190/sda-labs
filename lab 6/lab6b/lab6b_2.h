#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct DoubleWarehouseRecord {
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

// Node structure for the double linked list
struct DoubleNode {
    struct DoubleWarehouseRecord data;
    struct DoubleNode *prev;
    struct DoubleNode *next;
};

// Function prototypes
struct DoubleNode *createDoubleNode(struct DoubleWarehouseRecord data);
void insertDoubleNode(struct DoubleNode **head, int position, struct DoubleWarehouseRecord data);
void deleteDoubleNode(struct DoubleNode **head, int position);
void displayDoubleList(struct DoubleNode *head);
void destroyDoubleList(struct DoubleNode **head);
void saveDoubleListToFile(struct DoubleNode *head, const char *filename);
void readDoubleListFromFile(struct DoubleNode **head, const char *filename);
char *readDoubleFilename();

void doubleLinkedListProgram() {
    struct DoubleNode *head = NULL;

    int num_records;
    printf("Enter the number of records to enter: ");
    scanf("%d", &num_records);

    // Input warehouse registry data and create the double linked list
    for (int i = 0; i < num_records; i++) {
        struct DoubleWarehouseRecord record;
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

        insertDoubleNode(&head, i, record);
    }

    int choice;
    do {
        printf("\nDouble Warehouse Management System Menu:\n");
        printf("1. Display Warehouse Records\n");
        printf("2. Save Warehouse Records to File\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nWarehouse Records:\n");
                displayDoubleList(head);
                break;
            case 2: {
                char *filename = readDoubleFilename();
                saveDoubleListToFile(head, filename);
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

    // Destroy the double linked list
    destroyDoubleList(&head);
}

// Function to create a new node
struct DoubleNode *createDoubleNode(struct DoubleWarehouseRecord data) {
    struct DoubleNode *newNode = (struct DoubleNode *)malloc(sizeof(struct DoubleNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at a specified position
void insertDoubleNode(struct DoubleNode **head, int position, struct DoubleWarehouseRecord data) {
    if (position < 0) {
        printf("Invalid position.\n");
        return;
    }

    struct DoubleNode *newNode = createDoubleNode(data);

    if (*head == NULL) {
        *head = newNode;
        return;
    }

    if (position == 0) {
        newNode->next = *head;
        (*head)->prev = newNode;
        *head = newNode;
        return;
    }

    struct DoubleNode *current = *head;
    for (int i = 0; i < position - 1 && current->next != NULL; i++) {
        current = current->next;
    }

    newNode->next = current->next;
    if (current->next != NULL) {
        current->next->prev = newNode;
    }
    current->next = newNode;
    newNode->prev = current;
}

// Function to delete a node at a specified position
void deleteDoubleNode(struct DoubleNode **head, int position) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }

    if (position == 0) {
        struct DoubleNode *temp = *head;
        *head = (*head)->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
        free(temp);
        return;
    }

    struct DoubleNode *current = *head;
    for (int i = 0; i < position && current != NULL; i++) {
        current = current->next;
    }

    if (current == NULL) {
        printf("Position out of range.\n");
        return;
    }

    current->prev->next = current->next;
    if (current->next != NULL) {
        current->next->prev = current->prev;
    }
    free(current);
}

// Function to display the double linked list
void displayDoubleList(struct DoubleNode *head) {
    struct DoubleNode *current = head;
    while (current != NULL) {
        printf("Owner: %s %s\n", current->data.owner_name, current->data.owner_surname);
        printf("Product Name: %s\n", current->data.product_name);
        printf("Manufacturer: %s\n", current->data.manufacturer);
        printf("Contract Date: %d-%02d-%02d\n", current->data.contract_date_year, current->data.contract_date_month, current->data.contract_date_day);
        printf("Wholesale Price: %.2f\n", current->data.wholesale_price);
        printf("Unit Price: %.2f\n", current->data.unit_price);
        printf("Quantity: %d\n\n", current->data.quantity);
        current = current->next;
    }
}

// Function to destroy the double linked list and free memory
void destroyDoubleList(struct DoubleNode **head) {
    while (*head != NULL) {
        struct DoubleNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// Function to save the double linked list to a file
void saveDoubleListToFile(struct DoubleNode *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct DoubleNode *current = head;
    while (current != NULL) {
        fprintf(file, "Owner: %s %s\n", current->data.owner_name, current->data.owner_surname);
        fprintf(file, "Product Name: %s\n", current->data.product_name);
        fprintf(file, "Manufacturer: %s\n", current->data.manufacturer);
        fprintf(file, "Contract Date: %d-%02d-%02d\n", current->data.contract_date_year, current->data.contract_date_month, current->data.contract_date_day);
        fprintf(file, "Wholesale Price: %.2f\n", current->data.wholesale_price);
        fprintf(file, "Unit Price: %.2f\n", current->data.unit_price);
        fprintf(file, "Quantity: %d\n\n", current->data.quantity);
        current = current->next;
    }

    fclose(file);
}

// Function to read a filename from the user
char *readDoubleFilename() {
    char *filename = (char *)malloc(256 * sizeof(char));
    if (filename == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter filename: ");
    scanf("%s", filename);

    return filename;
}
