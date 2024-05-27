#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Queue_WarehouseRecord {
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

struct Queue_Node {
    struct Queue_WarehouseRecord data;
    struct Queue_Node *next;
};

struct Queue {
    struct Queue_Node *front;
    struct Queue_Node *rear;
};

struct Queue* Queue_createQueue();
void Queue_enqueue(struct Queue *q, struct Queue_WarehouseRecord record);
struct Queue_WarehouseRecord Queue_dequeue(struct Queue *q);
void Queue_displayQueue(struct Queue *q);
void Queue_insertRecord(struct Queue *q);
int Queue_searchRecordByValue(struct Queue *q, struct Queue_WarehouseRecord record);
int Queue_searchRecordByPosition(struct Queue *q, int position);
void Queue_deleteRecord(struct Queue *q, struct Queue_WarehouseRecord record);
void Queue_registerQueueToFile(struct Queue *q, char *filename);
char* Queue_readFilenameFromKeyboard();

struct Queue* Queue_createQueue() {
    struct Queue *q = (struct Queue*)malloc(sizeof(struct Queue));
    if (!q) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    q->front = q->rear = NULL;
    return q;
}

void Queue_enqueue(struct Queue *q, struct Queue_WarehouseRecord record) {
    struct Queue_Node *newNode = (struct Queue_Node*)malloc(sizeof(struct Queue_Node));
    if (!newNode) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = record;
    newNode->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

struct Queue_WarehouseRecord Queue_dequeue(struct Queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty!\n");
        exit(EXIT_FAILURE);
    }
    struct Queue_Node *temp = q->front;
    struct Queue_WarehouseRecord record = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return record;
}

void Queue_displayQueue(struct Queue *q) {
    if (q->front == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    struct Queue_Node *current = q->front;
    printf("Warehouse Records:\n");
    while (current != NULL) {
        printf("Owner: %s %s, Product Name: %s, Manufacturer: %s, Contract Date: %d-%02d-%02d\n",
               current->data.owner_name, current->data.owner_surname, current->data.product_name,
               current->data.manufacturer, current->data.contract_date_year, current->data.contract_date_month,
               current->data.contract_date_day);
        current = current->next;
    }
}

void Queue_insertRecord(struct Queue *q) {
    struct Queue_WarehouseRecord record;
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

    Queue_enqueue(q, record);
}

int Queue_searchRecordByValue(struct Queue *q, struct Queue_WarehouseRecord record) {
    struct Queue_Node *current = q->front;
    int position = 0;
    while (current != NULL) {
        if (strcmp(current->data.owner_name, record.owner_name) == 0 &&
            strcmp(current->data.owner_surname, record.owner_surname) == 0 &&
            strcmp(current->data.product_name, record.product_name) == 0 &&
            strcmp(current->data.manufacturer, record.manufacturer) == 0 &&
            current->data.contract_date_day == record.contract_date_day &&
            current->data.contract_date_month == record.contract_date_month &&
            current->data.contract_date_year == record.contract_date_year &&
            current->data.wholesale_price == record.wholesale_price &&
            current->data.unit_price == record.unit_price &&
            current->data.quantity == record.quantity) {
            return position;
        }
        position++;
        current = current->next;
    }
    return -1;
}

int Queue_searchRecordByPosition(struct Queue *q, int position) {
    struct Queue_Node *current = q->front;
    int pos = 0;
    while (current != NULL) {
        if (pos == position) {
            return pos;
        }
        pos++;
        current = current->next;
    }
    return -1;
}

void Queue_deleteRecord(struct Queue *q, struct Queue_WarehouseRecord record) {
    int position = Queue_searchRecordByValue(q, record);
    if (position == -1) {
        printf("Record not found!\n");
        return;
    }
    struct Queue_Node *current = q->front;
    struct Queue_Node *prev = NULL;
    int pos = 0;
    while (current != NULL) {
        if (pos == position) {
            if (prev == NULL) {
                q->front = current->next;
            } else {
                prev->next = current->next;
            }
            if (q->rear == current) {
                q->rear = prev;
            }
            free(current);
            printf("Record deleted successfully!\n");
            return;
        }
        prev = current;
        current = current->next;
        pos++;
    }
}

void Queue_registerQueueToFile(struct Queue *q, char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    struct Queue_Node *current = q->front;
    while (current != NULL) {
        fprintf(file, "Owner: %s %s, Product Name: %s, Manufacturer: %s, Contract Date: %d-%02d-%02d, Wholesale Price: %.2f, Unit Price: %.2f, Quantity: %d\n",
                current->data.owner_name, current->data.owner_surname, current->data.product_name,
                current->data.manufacturer, current->data.contract_date_year, current->data.contract_date_month,
                current->data.contract_date_day, current->data.wholesale_price, current->data.unit_price,
                current->data.quantity);
        current = current->next;
    }
    fclose(file);
    printf("Queue registered to file successfully!\n");
}

char* Queue_readFilenameFromKeyboard() {
    char *filename = (char*)malloc(sizeof(char) * 100);
    if (!filename) {
        printf("Memory allocation error!\n");
        exit(EXIT_FAILURE);
    }
    printf("Enter filename: ");
    scanf("%s", filename);
    return filename;
}

void Queue_queueProgram() {
    struct Queue *warehouseQueue = Queue_createQueue();
    char *filename;

    while (1) {
        int choice;
        printf("\n\n1. Insert Record\n");
        printf("2. Search Record\n");
        printf("3. Delete Record\n");
        printf("4. Display Warehouse Records\n");
        printf("5. Register Queue to File\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Queue_insertRecord(warehouseQueue);
                break;
            case 2:
                printf("Not implemented yet!\n");
                break;
            case 3:
                printf("Not implemented yet!\n");
                break;
            case 4:
                Queue_displayQueue(warehouseQueue);
                break;
            case 5:
                filename = Queue_readFilenameFromKeyboard();
                Queue_registerQueueToFile(warehouseQueue, filename);
                free(filename);
                break;
            case 6:
                printf("Exiting...\n");
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid choice!\n");
        }
    }
}
