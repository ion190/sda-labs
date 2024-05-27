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


void calculateTimeUntilContractExpires(struct WarehouseRecord records[], int num_records) {
    time_t now;
    time(&now);
    struct tm *current_time = localtime(&now);

    for (int i = 0; i < num_records; i++) {
        struct tm contract_time = {0};
        contract_time.tm_year = records[i].contract_date_year - 1900;
        contract_time.tm_mon = records[i].contract_date_month - 1;
        contract_time.tm_mday = records[i].contract_date_day;

        time_t contract_timestamp = mktime(&contract_time);
        double seconds_diff = difftime(contract_timestamp, now);

        records[i].remaining_years = (int)(seconds_diff / (365.25 * 24 * 3600));
        records[i].remaining_months = (int)((seconds_diff - records[i].remaining_years * 365.25 * 24 * 3600) / (30.44 * 24 * 3600));
        records[i].remaining_days = (int)((seconds_diff - records[i].remaining_years * 365.25 * 24 * 3600 - records[i].remaining_months * 30.44 * 24 * 3600) / (24 * 3600));
    }
}

void sortRecords(struct WarehouseRecord records[], int num_records) {
    // Sort records based on remaining time until contract expires
    for (int i = 0; i < num_records - 1; i++) {
        for (int j = 0; j < num_records - i - 1; j++) {
            // Calculate total remaining days
            int total_remaining_days1 = records[j].remaining_years * 365 + records[j].remaining_months * 30 + records[j].remaining_days;
            int total_remaining_days2 = records[j + 1].remaining_years * 365 + records[j + 1].remaining_months * 30 + records[j + 1].remaining_days;

            if (total_remaining_days1 < total_remaining_days2) {
                struct WarehouseRecord temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }
}

void printRecordsToFile(struct WarehouseRecord records[], int num_records) {
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    // Define current_time here
    time_t now;
    time(&now);
    struct tm *current_time = localtime(&now);

    fprintf(output_file, "Records sorted by remaining time until contract expires (Descending order):\n\n");
    for (int i = 0; i < num_records; i++) {
        fprintf(output_file, "Record %d:\n", i + 1);
        fprintf(output_file, "Owner: %s %s\n", records[i].owner_name, records[i].owner_surname);
        fprintf(output_file, "Product Name: %s\n", records[i].product_name);
        fprintf(output_file, "Manufacturer: %s\n", records[i].manufacturer);
        fprintf(output_file, "Contract Date: %d-%02d-%02d\n", records[i].contract_date_year, records[i].contract_date_month, records[i].contract_date_day);
        fprintf(output_file, "Wholesale Price: %.2f\n", records[i].wholesale_price);
        fprintf(output_file, "Unit Price: %.2f\n", records[i].unit_price);
        fprintf(output_file, "Quantity: %d\n", records[i].quantity);
        fprintf(output_file, "Contract Expiration Date: %d-%02d-%02d\n", 
                current_time->tm_year + 1900 + records[i].remaining_years,
                current_time->tm_mon + 1 + records[i].remaining_months,
                current_time->tm_mday + records[i].remaining_days);
        fprintf(output_file, "Remaining Time: %d years, %d months, %d days\n\n",
                records[i].remaining_years, records[i].remaining_months, records[i].remaining_days);
    }

    fclose(output_file);
}


void copyOutputToExperiment() {
    // Open output.txt for reading
    FILE *output_file = fopen("output.txt", "r");
    if (output_file == NULL) {
        printf("Error opening output.txt!\n");
        return;
    }

    // Open experiment.txt for reading and writing
    FILE *experiment_file = fopen("experiment.txt", "r+");
    if (experiment_file == NULL) {
        printf("Error opening experiment.txt!\n");
        fclose(output_file);
        return;
    }

    // Open a temporary file for writing
    FILE *temp_file = fopen("temp.txt", "w");
    if (temp_file == NULL) {
        printf("Error creating temporary file!\n");
        fclose(output_file);
        fclose(experiment_file);
        return;
    }

    // Copy contents of output.txt to temporary file
    int c;
    while ((c = fgetc(output_file)) != EOF) {
        fputc(c, temp_file);
    }

    // Append contents of experiment.txt to temporary file
    fseek(experiment_file, 0, SEEK_SET); // Move cursor to the beginning of experiment.txt
    while ((c = fgetc(experiment_file)) != EOF) {
        fputc(c, temp_file);
    }

    // Close all files
    fclose(output_file);
    fclose(experiment_file);
    fclose(temp_file);

    // Replace experiment.txt with the contents of the temporary file
    remove("experiment.txt");
    rename("temp.txt", "experiment.txt");

    printf("Output copied to experiment.txt successfully.\n");
}


int main() {
    // Create and open experiment.txt for writing
    FILE *experiment_file = fopen("experiment.txt", "w");
    if (experiment_file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int num_records;
    printf("Enter the number of records to enter: ");
    scanf("%d", &num_records);

    // Array to hold warehouse records
    struct WarehouseRecord records[num_records];

    // Input warehouse registry data
    for (int i = 0; i < num_records; i++) {
        printf("Record %d:\n", i+1);
        printf("Enter owner's name: ");
        scanf("%s", records[i].owner_name);
        printf("Enter owner's surname: ");
        scanf("%s", records[i].owner_surname);
        printf("Enter product name: ");
        scanf("%s", records[i].product_name);
        printf("Enter manufacturer: ");
        scanf("%s", records[i].manufacturer);
        printf("Enter contract conclusion date (day month year): ");
        scanf("%d %d %d", &records[i].contract_date_day, &records[i].contract_date_month, &records[i].contract_date_year);
        printf("Enter wholesale price: ");
        scanf("%f", &records[i].wholesale_price);
        printf("Enter unit price: ");
        scanf("%f", &records[i].unit_price);
        printf("Enter quantity (units): ");
        scanf("%d", &records[i].quantity);
    }

    // Input warehouse registry data and write to experiment.txt
    for (int i = 0; i < num_records; i++) {
        fprintf(experiment_file, "Record %d:\n", i + 1);
        fprintf(experiment_file, "Name: %s\nSurname: %s\nProduct Name: %s\nManufacturer: %s\nContract Date: %d-%d-%d\nWholesale Price: %.2f\nUnit Price: %.2f\nQuantity: %d\n",
                records[i].owner_name, records[i].owner_surname,
                records[i].product_name, records[i].manufacturer,
                records[i].contract_date_day, records[i].contract_date_month, records[i].contract_date_year,
                records[i].wholesale_price, records[i].unit_price, records[i].quantity);
        fprintf(experiment_file, "\n");
    }

    printf("----------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-8s | %-12s | %-20s | %-15s | %-12s | %-10s | %-10s | %-8s |\n", "Record", "Owner", "Product Name", "Manufacturer", "Contract Date", "Wholesale", "Unit", "Quantity");
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < num_records; i++) {
        printf("| %-8d | %-12s %s | %-20s | %-15s | %02d-%02d-%04d | %-10.2f | %-10.2f | %-8d |\n",
            i + 1, records[i].owner_name, records[i].owner_surname,
            records[i].product_name, records[i].manufacturer,
            records[i].contract_date_day, records[i].contract_date_month, records[i].contract_date_year,
            records[i].wholesale_price, records[i].unit_price, records[i].quantity);
    }
    printf("----------------------------------------------------------------------------------------------------------------------------------\n");



    // Open output.txt for writing
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }


    // Calculate remaining time until contract expires
    calculateTimeUntilContractExpires(records, num_records);
    // Sort records based on remaining time
    sortRecords(records, num_records);
    // Print sorted records to output.txt
    printRecordsToFile(records, num_records);

    copyOutputToExperiment();

    fclose(output_file);
    fclose(experiment_file);
    return 0;
}
