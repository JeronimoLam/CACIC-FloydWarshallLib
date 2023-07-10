#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
    const size_t chunk = 128;
    char *buffer = *lineptr;
    size_t capacity = *n;
    size_t length = 0;

    // Allocate initial buffer if one does not exist
    if (buffer == NULL || capacity == 0) {
        capacity = chunk;
        buffer = malloc(capacity);
        if (buffer == NULL) {
            return -1; // allocation failed
        }
    }

    // Read the line character by character
    int c;
    while ((c = fgetc(stream)) != EOF) {
        // Resize the buffer if we exceed capacity
        if (length + 1 > capacity) {
            // Double the buffer size
            capacity *= 2;
            buffer = realloc(buffer, capacity);
            if (buffer == NULL) {
                return -1; // allocation failed
            }
        }

        // Store the character and increment length
        buffer[length++] = c;

        // Stop at newline
        if (c == '\n') {
            break;
        }
    }

    // Null terminate the string
    if (length + 1 > capacity) {
        // Resize the buffer if we've run out of space for the null terminator
        capacity++;
        buffer = realloc(buffer, capacity);
        if (buffer == NULL) {
            return -1; // allocation failed
        }
    }
    buffer[length] = '\0';

    // Return the buffer and its size via output parameters
    *lineptr = buffer;
    *n = capacity;

    // If we read no characters return -1
    if(length == 0) {
        return -1;
    }

    // Otherwise, return the number of characters read, including newline, but not including null terminator
    return length;
}

static char *trim(char *str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

int CSV_calculateMatrixSize(FILE *file) {
    char c;
    int N = 1; // Start at 1 to count the first column

    while(((c = getc(file)) != '\n') && (c != EOF)){
        if(c == ','){
            N++;
        }
    }

    rewind(file);  // Reset the file pointer to the beginning of the file

    return N;
}

int* CSV_createMatrix(FILE* file, int size) {
    int *matrix = (int *)malloc(size * size * sizeof(int));

    char *line = NULL;
    size_t len = 0;
    char *token;
    int row = 0, col = 0;

    while ((getline(&line, &len, file)) != -1) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Split line into tokens (cells)
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);  // You need to ensure the trim() function is defined
            matrix[row * size + col] = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    if (line)
        free(line);

    return matrix;
}