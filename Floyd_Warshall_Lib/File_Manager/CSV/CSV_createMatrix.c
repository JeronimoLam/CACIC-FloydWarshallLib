#include <float.h>

#include "CSV_Utils.h"

static void *createIntMatrix(FILE *, unsigned int, unsigned int);
static void *createFloatMatrix(FILE *, unsigned int, unsigned int);
static void *createDoubleMatrix(FILE *, unsigned int, unsigned int);
static void *createCharMatrix(FILE *, unsigned int, unsigned int);

void *CSV_createMatrix(FW_Matrix FW, FILE *file)
{
    switch (FW.datatype)
    {
    case TYPE_INT:
        return createIntMatrix(file, FW.norm_size, FW.BS);
    case TYPE_FLOAT:
        return createFloatMatrix(file, FW.norm_size, FW.BS);
    case TYPE_DOUBLE:
        return createDoubleMatrix(file, FW.norm_size, FW.BS);

    default:
        return NULL;
    }
}

// These functions are similar but each one uses atoi, atof, strtod, or a simple assignment, respectively.
static void *createIntMatrix(FILE *file, unsigned int size, unsigned int BS) {
    rewind(file);
    int *matrix = (int *)malloc(size * size * sizeof(int));
    char line[2048];
    char *token;
    int row = 0, col = 0;

    // Initialize while reading the file
    for (row = 0; row < size; ++row) {
        col = 0; // Reset column counter for each row
        int readPartialLine = 0; // Flag to indicate if we have a partial line

        do {
            if (fgets(line, sizeof(line), file)) {
                // Check if fgets read the entire line or just a part of it
                char* newlinePos = strchr(line, '\n');
                if (newlinePos != NULL) {
                    *newlinePos = '\0'; // Terminate the string at the newline character, effectively removing it
                    readPartialLine = 0; // We've read the full line
                } else if (!feof(file)) {
                    readPartialLine = 1; // The line was too long and got cut off, so we have a partial line
                }

                token = strtok(line, ",");
                while (token != NULL && col < size) {
                    char *trimmedToken = trim(token);
                    if (strcmp(trimmedToken, "INF") == 0 || atoi(trimmedToken) == -1) {
                        matrix[row * size + col] = INT_MAX;
                    } else {
                        matrix[row * size + col] = atoi(trimmedToken);
                    }
                    token = strtok(NULL, ",");
                    col++;
                }
            }
        } while (readPartialLine); // Continue if we have a partial line

        // Fill remaining columns in the current row with INT_MAX
        for (; col < size; ++col) {
            matrix[row * size + col] = INT_MAX;
        }
    }

    return reorganizeToBlocks(matrix, size, BS, TYPE_INT);
}

static void *createFloatMatrix(FILE *file, unsigned int size, unsigned int BS)
{
    float *matrix = (float *)malloc(size * size * sizeof(float));
    char *token;
    int row = 0, col = 0;
    char line[2048];

    for (row = 0; row < size; ++row)
    {
        if (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = '\0';
            token = strtok(line, ",");
            col = 0;
            while (token != NULL && col < size)
            {
                char *trimmedToken = trim(token);
                if (strcmp(trimmedToken, "INF") == 0 || atof(trimmedToken) == -1.0)
                {
                    matrix[row * size + col] = FLT_MAX;
                }
                else
                {
                    matrix[row * size + col] = atof(trimmedToken);
                }
                token = strtok(NULL, ",");
                col++;
            }
        }
        // Fill remaining columns in the current row with FLOAT_MAX
        for (; col < size; ++col)
        {
            matrix[row * size + col] = FLT_MAX;
        }
        col = 0; // Reset column counter for the next row
    }

    return reorganizeToBlocks(matrix, size, BS, TYPE_FLOAT);
}

static void *createDoubleMatrix(FILE *file, unsigned int size, unsigned int BS)
{
    double *matrix = (double *)malloc(size * size * sizeof(double));
    char *token;
    int row = 0, col = 0;
    char line[2048];

    for (row = 0; row < size; ++row)
    {
        if (fgets(line, sizeof(line), file))
        {
            line[strcspn(line, "\n")] = '\0';
            token = strtok(line, ",");
            col = 0;
            while (token != NULL && col < size)
            {
                    char* trimmedToken = trim(token);

                if (strcmp(trimmedToken, "INF") == 0 || strtod(trimmedToken, NULL) == -1.0) {
                    matrix[row * size + col] = DBL_MAX;
                } else {
                    matrix[row * size + col] = strtod(token, NULL);
                }
                token = strtok(NULL, ",");
                col++;
            }
        }
        // Fill remaining columns in the current row with DBL_MAX
        for (; col < size; ++col)
        {
            matrix[row * size + col] = DBL_MAX;
        }
        col = 0; // Reset column counter for the next row
    }

    return reorganizeToBlocks(matrix, size, BS, TYPE_DOUBLE);
}