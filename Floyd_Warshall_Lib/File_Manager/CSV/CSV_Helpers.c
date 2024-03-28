#include "CSV_Utils.h"

// Trims the string
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


// Function to read the first line of a CSV file block by block and return the number of elements in it.
int CSV_calculateMatrixSize(FILE *file) {
    int count = 0; // Counter for the number of elements.
    int inElement = 0; // Flag to check if we are currently inside an element (number).
    char buffer[1024]; // Buffer to read the file.
    size_t bytesRead; // How many bytes were read in each iteration.

    // Read the file in blocks of 1024 bytes.
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytesRead; ++i) {
            // If we find a newline character, we are done.
            if (buffer[i] == '\n') {
                if (inElement) count++; // Make sure to count the last element before the newline.
                return count;
            }

            // If we find a comma and we are inside an element, increment the counter and mark that we are leaving the element.
            if (buffer[i] == ',' && inElement) {
                count++;
                inElement = 0;
            } else if (buffer[i] != ',' && buffer[i] != '\r' && buffer[i] != ' ') {
                // If we find a character that is not a comma, carriage return, or space, mark that we are inside an element.
                inElement = 1;
            }
        }
    }

    // In case we reach the end of the file without finding a newline, there might still be one last element to count.
    if (inElement) count++;

    return count;
}

// Scans the file looking for a '.' to determine if the data type is INT
// If '.' is found, the data type is set to FLOAT
DataType CSV_AutoDetectDataType(FILE * file) {
    char c;

    while(((c = getc(file)) != EOF)){
        if(c == '.'){
            return TYPE_DOUBLE;
        }
    }
    return TYPE_INT;
}

// Utility function to check if a character is considered a delimiter for CSV
int isDelimiter(char ch) {
    return ch == ',' || ch == '\n' || ch == EOF;
}

// Function to dynamically allocate and read the next token (element) from the file
char* readNextToken(FILE* file) {
    size_t capacity = 10; // Initial capacity
    size_t len = 0; // Current length of the token
    char* token = (char*)malloc(capacity * sizeof(char));
    if (!token) return NULL; // Allocation failed

    char ch;
    while ((ch = fgetc(file)) != EOF && !isDelimiter(ch)) {
        // Resize token buffer if necessary
        if (len + 1 >= capacity) {
            capacity *= 2; // Double the capacity
            char* newToken = (char*)realloc(token, capacity * sizeof(char));
            if (!newToken) {
                free(token);
                exit(10); // Reallocation failed
            }
            token = newToken;
        }

        // Append the character to the token
        token[len++] = ch;
    }

    if (len == 0 && ch == EOF) { // No more tokens
        free(token);
        return NULL;
    }

    // Null-terminate the token
    token[len] = '\0';
    return token;
}

// Converts the token to an integer, handling special cases
int tokenToInt(char* token) {
    if (strcmp(token, "INF") == 0 || atoi(token) == -1) {
        return INT_MAX;
    }
    return atoi(token);
}