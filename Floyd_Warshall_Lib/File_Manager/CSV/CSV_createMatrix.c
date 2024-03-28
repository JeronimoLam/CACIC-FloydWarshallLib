#include <float.h>

#include "CSV_Utils.h"

void* createIntMatrix(FILE* file, unsigned int size, unsigned int norm_size, int BS);

static void *createFloatMatrix(FILE *, unsigned int, unsigned int);
static void *createDoubleMatrix(FILE *, unsigned int, unsigned int);
static void *createCharMatrix(FILE *, unsigned int, unsigned int);

void *CSV_createMatrix(FW_Matrix FW, FILE *file)
{
    switch (FW.datatype)
    {
    case TYPE_INT:
        return createIntMatrix(file,FW.size, FW.norm_size, FW.BS);
        //return createIntMatrix(file, FW.norm_size, FW.BS);
    case TYPE_FLOAT:
        return createFloatMatrix(file, FW.norm_size, FW.BS);
    case TYPE_DOUBLE:
        return createDoubleMatrix(file, FW.norm_size, FW.BS);

    default:
        return NULL;
    }
}

void* createIntMatrix(FILE* file, unsigned int size, unsigned int norm_size, int BS) {
    rewind(file); // Ensure we start reading from the beginning of the file
    
    int* matrix = (int*)malloc(norm_size * norm_size * sizeof(int));
    if (!matrix) exit(9); // Allocation failed

    unsigned int row = 0, col = 0;
    char* token;
    
    for (unsigned int i = 0; i < norm_size * norm_size; i++) {
        if (row < size && col < size) {
            // Only attempt to read new tokens if within the original matrix size
                token = readNextToken(file);
                if (token) {
                    matrix[i] = tokenToInt(token);
                    // free(token); // Free the token now that we're done with it
                    if (++col == norm_size) {
                        col = 0;
                        row++;
                    }
                    continue;
                }
        }
        // For cells outside the original matrix or after file content ends, fill with INT_MAX
        matrix[i] = INT_MAX;
        if (++col == norm_size) {
            col = 0;
            row++;
        }
    }

    int * matrix2 = reorganizeToBlocks(matrix, norm_size, BS, TYPE_INT);
    return matrix2;
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