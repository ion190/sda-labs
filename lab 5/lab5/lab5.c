#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    ssize_t chars_read = 0;
    ssize_t bufsize = 0;
    int c;

    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }

    // Allocate memory if *lineptr is NULL or bufsize is 0
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = (char *)malloc(*n);
        if (*lineptr == NULL) {
            return -1; // Allocation failed
        }
    }

    // Read characters from stream until newline or EOF
    while ((c = fgetc(stream)) != EOF) {
        // Resize buffer if necessary
        if (chars_read >= *n - 1) {
            bufsize = *n * 2;
            char *temp = (char *)realloc(*lineptr, bufsize);
            if (temp == NULL) {
                return -1; // Allocation failed
            }
            *lineptr = temp;
            *n = bufsize;
        }

        (*lineptr)[chars_read++] = (char)c;

        if (c == '\n') {
            break; // Stop reading at newline
        }
    }

    if (chars_read == 0) {
        return -1; // No characters read
    }

    (*lineptr)[chars_read] = '\0'; // Null-terminate the string

    return chars_read; // Return the number of characters read
}


// Function to check if a character is a sentence terminator
int isSentenceTerminator(char c) {
    return (c == '.' || c == '!' || c == '?');
}

// Function to separate sentences and write them to output file
void separateSentences(const char *outputFileName, char *input_string, int length) {
    FILE *outputFile = fopen(outputFileName, "w");

    char sentence[1024];
    int index = 0;
    int nrOfStrings = 0;

    for (int i = 0; i < length; i++) {
        char c = input_string[i];
        if (c != '.' && c != '?' && c != '!') {
            sentence[index++] = c;
        } else {
            sentence[index] = '\0'; // Null-terminate the string
            fprintf(outputFile, "%s\n", sentence); // Write the sentence to the output file
            index = 0; // Reset the index for the next sentence
            nrOfStrings++;
        }
    }

    
    printf("Number of strings: %d", nrOfStrings);
    fprintf(outputFile, "%d\n", nrOfStrings);
    fclose(outputFile);
}

int main() {
    // Task 1
    // Create a file pointer
    FILE *file_ptr;

    // Open/create the file in write mode
    file_ptr = fopen("C:/Users/MySurfacePro/Desktop/Archive/SDA/lab 5/lab5/input.txt", "w");

    // Check if the file was opened successfully
    if (file_ptr == NULL) {
        printf("Error opening file!\n");
        return 1; // Exit with error
    }


    // Task 2
    char *input_string = NULL;
    size_t bufsize = 1024; // Initial buffer size
    input_string = (char *)malloc(bufsize * sizeof(char));

    if (input_string == NULL) {
        printf("Memory allocation failed!\n");
        return 1; // Exit with error
    }

    // Read string from keyboard
    printf("Input string\n");
    size_t len = 0;
    int c;
    while ((c = getchar()) != EOF && c != '\n') {
        input_string[len++] = c;
        if (len == bufsize) {
            bufsize *= 2;
            input_string = (char *)realloc(input_string, bufsize * sizeof(char));
            if (input_string == NULL) {
                printf("Memory reallocation failed!\n");
                return 1; // Exit with error
            }
        }
    }
    input_string[len] = '\0'; // Add null terminator

    // Write the string to the file
    fputs(input_string, file_ptr);

    int length = strlen(input_string);

    // Task 3 & 4
    file_ptr = fopen("C:/Users/MySurfacePro/Desktop/lab5/output.txt", "w");
    separateSentences("C:/Users/MySurfacePro/Desktop/lab5/output.txt", input_string, length);



    free(input_string);
    // Close the file
    fclose(file_ptr);

    return 0;
}
