#include "matrix.h"

int calculateMatrixSize(FILE *file) {
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

ssize_t getline(char **lineptr, size_t *n, FILE *stream) {
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

char *trim(char *str)
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


void print_matrix() {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%d ", matrix[i*size + j]);
        }
        printf("\n");
    }
}
