#include "common_functions.h"



static unsigned int maxDecimalLength = 0;

unsigned int getMaxDecimalLength()
{
    return maxDecimalLength;
}

void setMaxDecimalLength(unsigned int length)
{
    maxDecimalLength = length;
}

// Trims the string
char *trim(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}


// Utility function to check if a character is considered a delimiter for CSV
int isDelimiter(char ch)
{
    return ch == ',' || ch == '\n' || ch == EOF;
}


// Converts the token to an integer, handling special cases
int tokenToInt(char *token)
{
    if (strcmp(token, "INF") == 0 || atoi(token) == -1)
    {
        return INT_MAX;
    }
    return atoi(token);
}

// Nueva función que convierte el token a float
float tokenToFloat(char *token)
{
    if (strcmp(token, "INF") == 0 || atof(token) == -1.0)
    {
        return FLT_MAX;
    }
    return atof(token);
}

// Convierte el token a double
double tokenToDouble(char *token)
{
    if (strcmp(token, "INF") == 0 || atof(token) == -1.0)
    {
        return DBL_MAX;
    }
    return atof(token);
}

// Function to dynamically allocate and read the next token (element) from the file
char *readNextToken(FILE *file)
{
    size_t capacity = 10; // Initial capacity
    size_t len = 0;       // Current length of the token
    char *token = (char *)malloc(capacity * sizeof(char));
    if (!token)
        return NULL; // Allocation failed

    char ch;
    while ((ch = fgetc(file)) != EOF && !isDelimiter(ch))
    {
        // Resize token buffer if necessary
        if (len + 1 >= capacity)
        {
            capacity *= 2; // Double the capacity
            char *newToken = (char *)realloc(token, capacity * sizeof(char));
            if (!newToken)
            {
                free(token);
                exit(10); // Reallocation failed
            }
            token = newToken;
        }

        // Append the character to the token
        token[len++] = ch;
    }

    if (len == 0 && ch == EOF)
    { // No more tokens
        free(token);
        return NULL;
    }

    // Null-terminate the token
    token[len] = '\0';

    // Calculo la cantidad de lugares decimales
    char *dotPosition;

    if (token)
    {
        token = trim(token);
        char *dotPosition = strchr(token, '.');
        if (dotPosition != NULL)
        {
            unsigned int decimalLength = strlen(dotPosition + 1);

            if (decimalLength > maxDecimalLength)
            {
                maxDecimalLength = decimalLength;
            }
        }
    }
    return token;
}