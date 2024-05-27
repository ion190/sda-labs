#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct SimpleWarehouseRecord {
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

// Node structure for the linked list
struct SimpleNode {
    struct SimpleWarehouseRecord data;
    struct SimpleNode *next;
};

// Function prototypes
struct SimpleNode *createSimpleNode(struct SimpleWarehouseRecord data);
void insertSimpleNode(struct SimpleNode **head, int position, struct SimpleWarehouseRecord data);
void deleteSimpleNode(struct SimpleNode **head, int position);
void displaySimpleList(struct SimpleNode *head);
void destroySimpleList(struct SimpleNode **head);
void saveSimpleListToFile(struct SimpleNode *head, const char *filename);
void readSimpleListFromFile(struct SimpleNode **head, const char *filename);
char *readSimpleFilename();

void simpleLinkedListProgram() {
    struct SimpleNode *head = NULL;

    int num_records;
    printf("Enter the number of records to enter: ");
    scanf("%d", &num_records);

    // Input warehouse registry data and create the linked list
    for (int i = 0; i < num_records; i++) {
        struct SimpleWarehouseRecord record;
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

        insertSimpleNode(&head, i, record);
    }

    int choice;
    do {
        printf("\nSimple Warehouse Management System Menu:\n");
        printf("1. Display Warehouse Records\n");
        printf("2. Save Warehouse Records to File\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nWarehouse Records:\n");
                displaySimpleList(head);
                break;
            case 2: {
                char *filename = readSimpleFilename();
                saveSimpleListToFile(head, filename);
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

    // Destroy the linked list
    destroySimpleList(&head);
}


// Function to create a new node
struct SimpleNode *createSimpleNode(struct SimpleWarehouseRecord data) {
    struct SimpleNode *newNode = (struct SimpleNode *)malloc(sizeof(struct SimpleNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at a specified position
void insertSimpleNode(struct SimpleNode **head, int position, struct SimpleWarehouseRecord data) {
    if (position < 0) {
        printf("Invalid position.\n");
        return;
    }

    struct SimpleNode *newNode = createSimpleNode(data);

    if (position == 0) {
        newNode->next = *head;
        *head = newNode;
    } else {
        struct SimpleNode *temp = *head;
        for (int i = 0; i < position - 1 && temp != NULL; i++) {
            temp = temp->next;
        }
        if (temp == NULL) {
            printf("Position out of range.\n");
            return;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}

// Function to delete a node at a specified position
void deleteSimpleNode(struct SimpleNode **head, int position) {
    if (*head == NULL) {
        printf("List is empty.\n");
        return;
    }

    struct SimpleNode *temp = *head;
    if (position == 0) {
        *head = temp->next;
        free(temp);
    } else {
        struct SimpleNode *prev = NULL;
        for (int i = 0; i < position && temp != NULL; i++) {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            printf("Position out of range.\n");
            return;
        }
        prev->next = temp->next;
        free(temp);
    }
}

// Function to display the linked list
void displaySimpleList(struct SimpleNode *head) {
    struct SimpleNode *current = head;
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

// Function to destroy the linked list and free memory
void destroySimpleList(struct SimpleNode **head) {
    while (*head != NULL) {
        struct SimpleNode *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

// Function to save the linked list to a file
void saveSimpleListToFile(struct SimpleNode *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    struct SimpleNode *current = head;
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
char *readSimpleFilename() {
    char *filename = (char *)malloc(256 * sizeof(char));
    if (filename == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    printf("Enter filename: ");
    scanf("%s", filename);

    return filename;
}
