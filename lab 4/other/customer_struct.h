#include <stdlib.h>
#include <stdio.h>
struct Date {
    int day;
    int month;
    int year;
};

enum ApartmentType {
    SINGLE,
    DOUBLE,
    LUXURY
};


struct Customer {
    int id;
    char name[50];
    char surname[50];
    struct Date arrivalDate;
    struct Date departureDate;
    enum ApartmentType apartmentType;
    char sharing; // 'Y' for Yes, 'N' for No
};

void inputCustomers(struct Customer customers[], int numCustomers) {
    for (int i = 0; i < numCustomers; i++) {
        printf("\nEnter details for customer %d:\n", i + 1);
        customers[i].id = i + 1;
        printf("Name: ");
        scanf("%s", customers[i].name);
        printf("Surname: ");
        scanf("%s", customers[i].surname);
        printf("Arrival date (DD MM YYYY): ");
        scanf("%d %d %d", &customers[i].arrivalDate.day, &customers[i].arrivalDate.month, &customers[i].arrivalDate.year);
        printf("Departure date (DD MM YYYY): ");
        scanf("%d %d %d", &customers[i].departureDate.day, &customers[i].departureDate.month, &customers[i].departureDate.year);
        printf("Apartment type (0 for single, 1 for double, 2 for luxury): ");
        scanf("%d", &customers[i].apartmentType);
        printf("Accept sharing of apartment space with another guest? (Y/N): ");
        scanf(" %c", &customers[i].sharing);
    }
}

void displayCustomers(struct Customer customers[], int numCustomers) {
    for (int i = 0; i < numCustomers; i++) {
        printf("ID: %d, Name: %s, Surname: %s, Arrival Date: %02d/%02d/%d, Departure Date: %02d/%02d/%d, Apartment Type: %d, Sharing: %c\n",
               customers[i].id, customers[i].name, customers[i].surname, customers[i].arrivalDate.day, customers[i].arrivalDate.month, customers[i].arrivalDate.year,
               customers[i].departureDate.day, customers[i].departureDate.month, customers[i].departureDate.year, customers[i].apartmentType, customers[i].sharing);
    }
}

void swap(struct Customer *a, struct Customer *b) {
    struct Customer temp = *a;
    *a = *b;
    *b = temp;
}

int partitionByName(struct Customer customers[], int low, int high) {
    char pivot[50];
    strcpy(pivot, customers[high].name);
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(customers[j].name, pivot) < 0) {
            i++;
            swap(&customers[i], &customers[j]);
        }
    }
    swap(&customers[i + 1], &customers[high]);
    return (i + 1);
}

void quickSortByName(struct Customer customers[], int low, int high) {
    if (low < high) {
        int pi = partitionByName(customers, low, high);
        quickSortByName(customers, low, pi - 1);
        quickSortByName(customers, pi + 1, high);
    }
}

void countingSort(struct Customer customers[], int numCustomers, int exp, int ascending) {
    struct Customer output[numCustomers];
    int count[10] = {0};

    // Count occurrences of each digit
    for (int i = 0; i < numCustomers; i++)
        count[((customers[i].departureDate.year / exp) % 10)]++;

    // Adjust count array to store actual positions
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array, maintaining stability
    for (int i = numCustomers - 1; i >= 0; i--) {
        output[count[((customers[i].departureDate.year / exp) % 10)] - 1] = customers[i];
        count[((customers[i].departureDate.year / exp) % 10)]--;
    }

    // Reset count array for next pass
    for (int i = 0; i < 10; i++)
        count[i] = 0;

    // Copy the sorted output back to the original array
    for (int i = 0; i < numCustomers; i++)
        customers[i] = output[i];
}

void radixSortByDepartureDate(struct Customer customers[], int numCustomers) {
    int maxYear = customers[0].departureDate.year;
    for (int i = 1; i < numCustomers; i++) {
        if (customers[i].departureDate.year > maxYear)
            maxYear = customers[i].departureDate.year;
    }

    for (int exp = 1; maxYear / exp > 0; exp *= 10)
        countingSort(customers, numCustomers, exp, 0); // Sorting in descending order
}

void modifyCustomer(struct Customer customers[], int numCustomers) {
    char name[50];
    char surname[50];
    int year;

    printf("\nEnter name, surname, and year to modify the customer record:\n");
    printf("Name: ");
    scanf("%s", name);
    printf("Surname: ");
    scanf("%s", surname);
    printf("Year of birth: ");
    scanf("%d", &year);

    for (int i = 0; i < numCustomers; i++) {
        if (strcmp(customers[i].name, name) == 0 && strcmp(customers[i].surname, surname) == 0 && customers[i].departureDate.year == year) {
            printf("\nCustomer found. Enter new details:\n");
            printf("Arrival date (DD MM YYYY): ");
            scanf("%d %d %d", &customers[i].arrivalDate.day, &customers[i].arrivalDate.month, &customers[i].arrivalDate.year);
            printf("Departure date (DD MM YYYY): ");
            scanf("%d %d %d", &customers[i].departureDate.day, &customers[i].departureDate.month, &customers[i].departureDate.year);
            printf("Apartment type (0 for single, 1 for double, 2 for luxury): ");
            scanf("%d", &customers[i].apartmentType);
            printf("Accept sharing of apartment space with another guest? (Y/N): ");
            scanf(" %c", &customers[i].sharing);
            printf("Record modified successfully.\n");
            return;
        }
    }
    printf("Customer not found.\n");
}