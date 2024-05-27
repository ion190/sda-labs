#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "customer_struct.h"
#include "union.h"

#define MAX_CUSTOMERS 100

int main() {
    int choice;

    printf("Enter choice (1 for union 2 for struct): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: {
            union Guest guests[MAX_CUSTOMERS];
            int num_guests;

            printf("Enter the number of guests: ");
            scanf("%d", &num_guests);

            // Ensure you have functions like input_guests and display_guests defined
            input_guests(guests, num_guests);

            printf("\n--- Guests before sorting ---\n");
            display_guests(guests, num_guests);

            // Sorting guests by departure date using radix sort
            radix_sort_by_departure_date(guests, num_guests);
            quick_sort_by_name(guests, 0, num_guests-1);
            printf("\n Guests after sorting by name (ascending):\n");
            display_guests(guests, num_guests);
            printf("\n Guests after sorting by departure date (descending):\n");
            display_guests(guests, num_guests);

            modify_guest(guests, num_guests);

            printf("\n Guests after modification:\n");
            display_guests(guests, num_guests);
            break;
        }

        case 2: {
            struct Customer customers[MAX_CUSTOMERS];
            int numCustomers;

            printf("Enter the number of customers: ");
            scanf("%d", &numCustomers);

            inputCustomers(customers, numCustomers);

            printf("\nCustomers entered:\n");
            displayCustomers(customers, numCustomers);

            // Sort by name using Quick Sort
            quickSortByName(customers, 0, numCustomers - 1);
            printf("\nCustomers sorted by name (ascending):\n");
            displayCustomers(customers, numCustomers);

            // Sort by departure date using Radix Sort
            radixSortByDepartureDate(customers, numCustomers);
            printf("\nCustomers sorted by departure date (descending):\n");
            displayCustomers(customers, numCustomers);

            // Modify a customer record
            modifyCustomer(customers, numCustomers);
            break;
        }
        
        default:
            printf("Invalid choice.\n");
    }

    return 0;
}

    



