#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "version2.h"

union departureTime {
    unsigned int hour : 5; // 0-23
    unsigned int minutes : 6; // 0-59
};

union departureDate {
    unsigned int day : 5; // 1-31
    unsigned int month : 4; // 1-12
    unsigned int year : 12; // year
};

union customerUnion {
    char name[30];
    char surname[30];
    int seatNo;
    int seatPrice;
    int flightNo;
    char destination[50]; // Array of characters for destination

    union departureTime departureTime;
    union departureDate departureDate;
};

void readData(union customerUnion *data) {
    printf("Enter passenger's flight info:\n");
    printf("Name: ");
    scanf("%s", data->name);
    printf("Surname: ");
    scanf("%s", data->surname);
    printf("Seat no: ");
    scanf("%d", &data->seatNo);
    printf("Seat price: ");
    scanf("%d", &data->seatPrice);
    printf("Flight no: ");
    scanf("%d", &data->flightNo);
    printf("Destination: ");
    getchar(); // Consume the newline character left in the buffer
    fgets(data->destination, sizeof(data->destination), stdin); // Read destination with spaces
    data->destination[strcspn(data->destination, "\n")] = '\0'; // Remove trailing newline if present
    
    // Read departure time
    int tempHour, tempMinutes;
    printf("Departure time hour: ");
    scanf("%d", &tempHour);
    data->departureTime.hour = tempHour;
    printf("Departure time minutes: ");
    scanf("%d", &tempMinutes);
    data->departureTime.minutes = tempMinutes;
    
    // Read departure date
    int tempDay, tempMonth, tempYear;
    printf("Departure date day: ");
    scanf("%d", &tempDay);
    data->departureDate.day = tempDay;
    printf("Departure date month: ");
    scanf("%d", &tempMonth);
    data->departureDate.month = tempMonth;
    printf("Departure date year: ");
    scanf("%d", &tempYear);
    data->departureDate.year = tempYear;
}

void displayData(union customerUnion data[], int n) {
    printf("\nPassengers' flight information:\n");
    printf("%-4s %-15s %-15s %-8s %-10s %-12s %-12s %-10s %-15s\n", "No.", "Name", "Surname", "Seat No", "Price", "Flight No", "Destination", "Departure Time", "Departure Date");
    for (int i = 0; i < n; i++) {
        printf("%-4d %-15s %-15s %-8d %-10d %-12d %-12s %02d:%02d         %02d-%02d-%-4d\n", 
               i + 1, data[i].name, data[i].surname, data[i].seatNo, data[i].seatPrice, data[i].flightNo, data[i].destination, 
               data[i].departureTime.hour, data[i].departureTime.minutes, 
               data[i].departureDate.day, data[i].departureDate.month, data[i].departureDate.year);
    }
}


void bubbleSortSurname(union customerUnion data[], int n) {
    int swapped;
    union customerUnion temp;
    
    for (int i = 0; i < n - 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(data[j].surname, data[j + 1].surname) > 0) {
                // Swap data[j] and data[j + 1]
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
                swapped = 1;
            }
        }
        // If no two elements were swapped by the inner loop, then break
        if (swapped == 0)
            break;
    }
}

void radixSortDepartureDate(union customerUnion data[], int n) {
    // Perform counting sort for each digit (from most significant to least significant)
    int maxYear = 0, maxMonth = 0, maxDay = 0;
    for (int i = 0; i < n; i++) {
        if (data[i].departureDate.year > maxYear)
            maxYear = data[i].departureDate.year;
        if (data[i].departureDate.month > maxMonth)
            maxMonth = data[i].departureDate.month;
        if (data[i].departureDate.day > maxDay)
            maxDay = data[i].departureDate.day;
    }

    int exp = 1;
    union customerUnion output[n];
    while (maxYear / exp > 0) {
        int count[10] = {0};
        
        // Count occurrences of each digit in the current place
        for (int i = 0; i < n; i++) {
            int digit = (data[i].departureDate.year / exp) % 10;
            count[digit]++;
        }
        
        // Adjust count array to contain actual position of each digit in the output array
        for (int i = 8; i >= 0; i--) { // Adjusted loop for descending order
            count[i] += count[i + 1];
        }
        
        // Build the output array in reverse order to achieve descending sorting
        for (int i = n - 1; i >= 0; i--) {
            int digit = (data[i].departureDate.year / exp) % 10;
            output[count[digit] - 1] = data[i];
            count[digit]--;
        }
        
        // Copy the sorted elements back to the original array
        for (int i = 0; i < n; i++) {
            data[i] = output[i];
        }
        
        // Move to the next digit
        exp *= 10;
    }

    // Now sort by month if years are similar
    exp = 1;
    while (maxMonth / exp > 0) {
        int count[10] = {0};
        
        // Count occurrences of each digit in the current place
        for (int i = 0; i < n; i++) {
            int digit = (data[i].departureDate.month / exp) % 10;
            count[digit]++;
        }
        
        // Adjust count array to contain actual position of each digit in the output array
        for (int i = 8; i >= 0; i--) { // Adjusted loop for descending order
            count[i] += count[i + 1];
        }
        
        // Build the output array in reverse order to achieve descending sorting
        for (int i = n - 1; i >= 0; i--) {
            int digit = (data[i].departureDate.month / exp) % 10;
            output[count[digit] - 1] = data[i];
            count[digit]--;
        }
        
        // Copy the sorted elements back to the original array
        for (int i = 0; i < n; i++) {
            data[i] = output[i];
        }
        
        // Move to the next digit
        exp *= 10;
    }

    // Now sort by day if months are similar
    exp = 1;
    while (maxDay / exp > 0) {
        int count[10] = {0};
        
        // Count occurrences of each digit in the current place
        for (int i = 0; i < n; i++) {
            int digit = (data[i].departureDate.day / exp) % 10;
            count[digit]++;
        }
        
        // Adjust count array to contain actual position of each digit in the output array
        for (int i = 8; i >= 0; i--) { // Adjusted loop for descending order
            count[i] += count[i + 1];
        }
        
        // Build the output array in reverse order to achieve descending sorting
        for (int i = n - 1; i >= 0; i--) {
            int digit = (data[i].departureDate.day / exp) % 10;
            output[count[digit] - 1] = data[i];
            count[digit]--;
        }
        
        // Copy the sorted elements back to the original array
        for (int i = 0; i < n; i++) {
            data[i] = output[i];
        }
        
        // Move to the next digit
        exp *= 10;
    }
}

void modifyCustomerData(union customerUnion data[], int n) {
    char searchName[30];
    char searchSurname[30];
    int searchSeatNo;

    // Input customer details to search
    printf("Enter customer's name: ");
    scanf("%s", searchName);
    printf("Enter customer's surname: ");
    scanf("%s", searchSurname);
    printf("Enter customer's seat no: ");
    scanf("%d", &searchSeatNo);

    // Search for the customer in the register
    int foundIndex = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(data[i].name, searchName) == 0 && strcmp(data[i].surname, searchSurname) == 0 && data[i].seatNo == searchSeatNo) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Customer not found in the register.\n");
        return;
    }

    // Display menu for modification options
    printf("\nMenu for modification options:\n");
    printf("1. Modify seat price\n");
    printf("2. Modify flight number\n");
    printf("3. Modify destination\n");
    printf("4. Modify departure time\n");
    printf("5. Modify departure date\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    // Modify the selected information based on user's choice
    switch (choice) {
        case 1:
            printf("Enter new seat price: ");
            scanf("%d", &data[foundIndex].seatPrice);
            break;
        case 2:
            printf("Enter new flight number: ");
            scanf("%d", &data[foundIndex].flightNo);
            break;
        case 3:
            printf("Enter new destination: ");
            getchar(); // Consume the newline character left in the buffer
            fgets(data[foundIndex].destination, sizeof(data[foundIndex].destination), stdin); // Read destination with spaces
            data[foundIndex].destination[strcspn(data[foundIndex].destination, "\n")] = '\0'; // Remove trailing newline if present
            break;
        case 4:
            int tempHour, tempMinutes;
            printf("Enter new departure time hour: ");
            scanf("%d", &tempHour);
            data[foundIndex].departureTime.hour = tempHour;
            printf("Enter new departure time minutes: ");
            scanf("%d", &tempMinutes);
            data[foundIndex].departureTime.minutes = tempMinutes;
            break;
        case 5:
            int tempDay, tempMonth, tempYear;
            printf("Enter new departure date day: ");
            scanf("%d", &tempDay);
            data[foundIndex].departureDate.day = tempDay;
            printf("Enter new departure date month: ");
            scanf("%d", &tempMonth);
            data[foundIndex].departureDate.month = tempMonth;
            printf("Enter new departure date year: ");
            scanf("%d", &tempYear);
            data[foundIndex].departureDate.year = tempYear;
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }

    printf("Customer information modified successfully.\n");
}

int main() {
    int n;
    printf("Number of passengers: ");
    scanf("%d", &n);

    union customerUnion registerData[n];
    for (int i = 0; i < n; i++) {
        printf("\nCustomer %d:\n\n", i + 1);
        readData(&registerData[i]);
    }

    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Modify data\n");
        printf("2. Display register\n");
        printf("3. Sort register ascendingly (Bubble Sort)\n");
        printf("4. Sort register descendingly (Radix Sort)\n");
        printf("5. Close program\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                modifyCustomerData(registerData, n);
                break;
            case 2:
                displayData(registerData, n);
                break;
            case 3:
                // Sort register ascendingly
                bubbleSortSurname(registerData, n);
                printf("Register sorted ascendingly.\n");
                break;
            case 4:
                // Sort register descendingly
                radixSortDepartureDate(registerData, n);
                printf("Register sorted descendingly.\n");
                break;
            case 5:
                printf("Closing program...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);

    return 0;
}

