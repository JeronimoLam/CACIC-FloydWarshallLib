//
#include "JSON_Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int JSON_calculate_matrix_size(FILE *file)
{
    int size = 0;
    char line[100]; // JSONs attribute is before the 100th position

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Check if the line contains the "size" field
        if (strstr(line, "\"size\"") != NULL)
        {
            // Find the position of the colon
            char *colon = strchr(line, ':');
            if (colon != NULL)
            {
                // Find the position of the comma after the colon
                char *comma = strchr(colon, ',');
                if (comma != NULL)
                {
                    // Convert the string between the colon and the comma to an integer
                    size = atoi(colon + 1);
                }
            }
            break; // Stop reading the file after finding the "size" field
        }
    }

    return size;
}

DataType JSON_auto_detect_dataType(FILE *file)
{
    DataType dataType = UNDEFINED;
    char *type = NULL;
    char line[100]; // JSONs attribute is before the 100th position

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Check if the line contains the "Tipo" field
        if (strstr(line, "\"type\"") != NULL)
        {
            // Find the position of the colon
            char *colon = strchr(line, ':');
            if (colon != NULL)
            {
                // Find the position of the opening quote after the colon
                char *quote = strchr(colon, '\"');
                if (quote != NULL)
                {
                    // Find the position of the closing quote after the opening quote
                    char *closingQuote = strchr(quote + 1, '\"');
                    if (closingQuote != NULL)
                    {
                        // Calculate the length of the type string
                        size_t length = closingQuote - quote - 1;
                        // Allocate memory for the type string
                        type = malloc(length + 1);
                        if (type != NULL)
                        {
                            // Copy the type string into the allocated memory
                            strncpy(type, quote + 1, length);
                            type[length] = '\0'; // Null-terminate the string
                        }
                        else{
                            fprintf(stderr, "Error: Allocation failed.\n");
                            exit(EXIT_ALOCATION_FAILED); // Allocation failed
                        }
                    }
                }
            }
            break; // Stop reading the file after finding the "Tipo" field
        }
    }

    type = trim(type);

    // Retorna el correcto DataType segun type
    if (type != NULL)
    {
        if (strcmp(type, "int") == 0)
        {
            dataType = TYPE_INT;
        }
        else if (strcmp(type, "float") == 0)
        {
            dataType = TYPE_FLOAT;
        }
        else if (strcmp(type, "double") == 0)
        {
            dataType = TYPE_DOUBLE;
        }
    }

    free(type); // Free the allocated memory for the type string

    return dataType;
}
