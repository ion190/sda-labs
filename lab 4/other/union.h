#include <stdlib.h>
#include <stdio.h>

union Guest 
{
    struct {
    int id;
    char first_name[50];
    char last_name[50];
    struct Date arrival_date;
    struct Date departure_date;
    enum ApartmentType apartment_type;
    char sharing; // 'Y' for Yes, 'N' for No

};
int data[10]; // An array to accommodate all data fields, adjust size as needed
};
 

void input_guests(union Guest guests[], int num_guests) {
    for (int i = 0; i < num_guests; i++) {
        printf("\nEnter details for guest %d:\n", i + 1);
        guests[i].id = i + 1;
        printf("First Name: ");
        scanf("%s", guests[i].first_name);
        printf("Last Name: ");
        scanf("%s", guests[i].last_name);
        printf("Arrival date (DD MM YYYY): ");
        scanf("%d %d %d", &guests[i].arrival_date.day, &guests[i].arrival_date.month, &guests[i].arrival_date.year);
        printf("Departure date (DD MM YYYY): ");
        scanf("%d %d %d", &guests[i].departure_date.day, &guests[i].departure_date.month, &guests[i].departure_date.year);
        printf("Apartment type (0 for single, 1 for double, 2 for luxury): ");
        scanf("%d", &guests[i].apartment_type);
        printf("Accept sharing of apartment space with another guest? (Y/N): ");
        scanf(" %c", &guests[i].sharing);
    }
}

void display_guests(union Guest guests[], int num_guests) {
    for (int i = 0; i < num_guests; i++) {
        printf("ID: %d, First Name: %s, Last Name: %s, Arrival Date: %02d/%02d/%d, Departure Date: %02d/%02d/%d, Apartment Type: %d, Sharing: %c\n",
               guests[i].id, guests[i].first_name, guests[i].last_name, guests[i].arrival_date.day, guests[i].arrival_date.month, guests[i].arrival_date.year,
               guests[i].departure_date.day, guests[i].departure_date.month, guests[i].departure_date.year, guests[i].apartment_type, guests[i].sharing);
    }
}

void swap_guests(union Guest *a, union Guest *b) {
    union Guest temp = *a;
    *a = *b;
    *b = temp;
}

int partition_by_name(union Guest guests[], int low, int high) {
    char pivot[50];
    strcpy(pivot, guests[high].first_name);
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (strcmp(guests[j].first_name, pivot) < 0) {
            i++;
            swap_guests(&guests[i], &guests[j]);
        }
    }
    swap_guests(&guests[i + 1], &guests[high]);
    return (i + 1);
}

void quick_sort_by_name(union Guest guests[], int low, int high) {
    if (low < high) {
        int pi = partition_by_name(guests, low, high);
        quick_sort_by_name(guests, low, pi - 1);
        quick_sort_by_name(guests, pi + 1, high);
    }
}

void counting_sort(union Guest guests[], int num_guests, int exp, int ascending) {
    union Guest output[num_guests];
    int count[10] = {0};

    // Count occurrences of each digit
    for (int i = 0; i < num_guests; i++)
        count[((guests[i].departure_date.year / exp) % 10)]++;

    // Adjust count array to store actual positions
    for (int i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array, maintaining stability
    for (int i = num_guests - 1; i >= 0; i--) {
        output[count[((guests[i].departure_date.year / exp) % 10)] - 1] = guests[i];
        count[((guests[i].departure_date.year / exp) % 10)]--;
    }

    // Reset count array for next pass
    for (int i = 0; i < 10; i++)
        count[i] = 0;

    // Copy the sorted output back to the original array in descending order
    for (int i = 0; i < num_guests; i++)
        guests[i] = output[i];
}

void radix_sort_by_departure_date(union Guest guests[], int num_guests) {
    int max_year = guests[0].departure_date.year;
    for (int i = 1; i < num_guests; i++) {
        if (guests[i].departure_date.year > max_year)
            max_year = guests[i].departure_date.year;
    }

    for (int exp = 1; max_year / exp > 0; exp *= 10)
        counting_sort(guests, num_guests, exp, 0); // Sorting in descending order
}

void modify_guest(union Guest guests[], int num_guests) {
    char first_name[50];
    char last_name[50];
    int year;

    printf("\nEnter first name, last name, and year to modify the guest record:\n");
    printf("First Name: ");
    scanf("%s", first_name);
    printf("Last Name: ");
    scanf("%s", last_name);
    printf("Year of birth: ");
    scanf("%d", &year);

    for (int i = 0; i < num_guests; i++) {
        if (strcmp(guests[i].first_name, first_name) == 0 && strcmp(guests[i].last_name, last_name) == 0 && guests[i].departure_date.year == year) {
            printf("\nGuest found. Enter new details:\n");
            printf("Arrival date (DD MM YYYY): ");
            scanf("%d %d %d", &guests[i].arrival_date.day, &guests[i].arrival_date.month, &guests[i].arrival_date.year);
            printf("Departure date (DD MM YYYY): ");
            scanf("%d %d %d", &guests[i].departure_date.day, &guests[i].departure_date.month, &guests[i].departure_date.year);
            printf("Apartment type (0 for single, 1 for double, 2 for luxury): ");
            scanf("%d", &guests[i].apartment_type);
            printf("Accept sharing of apartment space with another guest? (Y/N): ");
            scanf(" %c", &guests[i].sharing);
            printf("Record modified successfully.\n");
            return;
        }
    }
    printf("Guest not found.\n");
}