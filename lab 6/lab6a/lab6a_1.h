#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct WarehouseRecord {
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

struct Node {
    struct WarehouseRecord data;
    struct Node* next;
};

struct Stack {
    struct Node* top;
};

struct Stack* createStack();
void push(struct Stack* stack, struct WarehouseRecord data);
struct WarehouseRecord pop(struct Stack* stack);
void displayStack(struct Stack* stack);
void insertRecord(struct Stack* stack);
int searchRecord(struct Stack* stack, char searchKey[]);
void deleteRecord(struct Stack* stack, int position);
void saveStackToFile(struct Stack* stack, char filename[]);
void readFilename(char filename[]);

void stackProgram() {
    struct Stack* stack = createStack();

    int choice;
    char filename[100];

    do {
        printf("\n\nStack Operations Menu:\n");
        printf("1. Display Stack\n");
        printf("2. Insert Record\n");
        printf("3. Search Record\n");
        printf("4. Delete Record\n");
        printf("5. Save Stack to File\n");
        printf("6. Read Filename\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayStack(stack);
                break;
            case 2:
                insertRecord(stack);
                break;
            case 3: {
                char searchKey[50];
                printf("Enter search key: ");
                scanf("%s", searchKey);
                int position = searchRecord(stack, searchKey);
                if (position == -1) {
                    printf("Record not found.\n");
                } else {
                    printf("Record found at position %d.\n", position);
                }
                break;
            }
            case 4: {
                int position;
                printf("Enter position to delete: ");
                scanf("%d", &position);
                deleteRecord(stack, position);
                break;
            }
            case 5:
                if (strcmp(filename, "") == 0) {
                    printf("Please enter a filename first.\n");
                } else {
                    saveStackToFile(stack, filename);
                    printf("Stack saved to file '%s'.\n", filename);
                }
                break;
            case 6:
                readFilename(filename);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice! Please enter a valid option.\n");
        }
    } while (choice != 0);

}

struct Stack* createStack() {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = NULL;
    return stack;
}

void push(struct Stack* stack, struct WarehouseRecord data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
}

struct WarehouseRecord pop(struct Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack underflow!\n");
        exit(1);
    }
    struct Node* temp = stack->top;
    struct WarehouseRecord poppedData = temp->data;
    stack->top = temp->next;
    free(temp);
    return poppedData;
}

void displayStack(struct Stack* stack) {
    if (stack->top == NULL) {
        printf("Stack is empty.\n");
        return;
    }
    struct Node* current = stack->top;
    printf("Warehouse Records (top to bottom):\n");
    while (current != NULL) {
        printf("Owner: %s %s\n", current->data.owner_name, current->data.owner_surname);
        printf("Product Name: %s\n", current->data.product_name);
        printf("Manufacturer: %s\n", current->data.manufacturer);
        printf("Contract Date: %d-%02d-%02d\n", current->data.contract_date_year, current->data.contract_date_month, current->data.contract_date_day);
        printf("Wholesale Price: %.2f\n", current->data.wholesale_price);
        printf("Unit Price: %.2f\n", current->data.unit_price);
        printf("Quantity: %d\n", current->data.quantity);
        printf("\n");
        current = current->next;
    }
}

void insertRecord(struct Stack* stack) {
    struct WarehouseRecord newRecord;
    printf("Enter owner's name: ");
    scanf("%s", newRecord.owner_name);
    printf("Enter owner's surname: ");
    scanf("%s", newRecord.owner_surname);
    printf("Enter product name: ");
    scanf("%s", newRecord.product_name);
    printf("Enter manufacturer: ");
    scanf("%s", newRecord.manufacturer);
    printf("Enter contract conclusion date (day month year): ");
    scanf("%d %d %d", &newRecord.contract_date_day, &newRecord.contract_date_month, &newRecord.contract_date_year);
    printf("Enter wholesale price: ");
    scanf("%f", &newRecord.wholesale_price);
    printf("Enter unit price: ");
    scanf("%f", &newRecord.unit_price);
    printf("Enter quantity (units): ");
    scanf("%d", &newRecord.quantity);

    push(stack, newRecord);
}

int searchRecord(struct Stack* stack, char searchKey[]) {
    int position = 0;
    struct Node* current = stack->top;
    while (current != NULL) {
        if (strcmp(current->data.product_name, searchKey) == 0) {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1;
}

void deleteRecord(struct Stack* stack, int position) {
    if (stack->top == NULL) {
        printf("Stack is empty.\n");
        return;
    }
    if (position < 1) {
        printf("Invalid position.\n");
        return;
    }
    struct Node* current = stack->top;
    struct Node* prev = NULL;
    int count = 1;
    while (current != NULL && count != position) {
        prev = current;
        current = current->next;
        count++;
    }
    if (current == NULL) {
        printf("Position exceeds the size of the stack.\n");
        return;
    }
    if (prev == NULL) {
        stack->top = current->next;
    } else {
        prev->next = current->next;
    }
    free(current);
}

void saveStackToFile(struct Stack* stack, char filename[]) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    struct Node* current = stack->top;
    while (current != NULL) {
        fprintf(file, "Owner: %s %s\n", current->data.owner_name, current->data.owner_surname);
        fprintf(file, "Product Name: %s\n", current->data.product_name);
        fprintf(file, "Manufacturer: %s\n", current->data.manufacturer);
        fprintf(file, "Contract Date: %d-%02d-%02d\n", current->data.contract_date_year, current->data.contract_date_month, current->data.contract_date_day);
        fprintf(file, "Wholesale Price: %.2f\n", current->data.wholesale_price);
        fprintf(file, "Unit Price: %.2f\n", current->data.unit_price);
        fprintf(file, "Quantity: %d\n", current->data.quantity);
        fprintf(file, "\n");
        current = current->next;
    }
    fclose(file);
}

void readFilename(char filename[]) {
    printf("Enter filename: ");
    scanf("%s", filename);
}
