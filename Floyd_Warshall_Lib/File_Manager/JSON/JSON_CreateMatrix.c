#include "JSON_Utils.h"

void *JSON_createIntMatrix(FILE *file, unsigned int size, unsigned int norm_size);
void *JSON_createFloatMatrix(FILE *file, unsigned int size, unsigned int norm_size);
void *JSON_createDoubleMatrix(FILE *file, unsigned int size, unsigned int norm_size);

void *JSON_createMatrix(FW_Matrix FW, FILE *file)
{
    switch (FW.datatype)
    {
    case TYPE_INT:
        return JSON_createIntMatrix(file, FW.size, FW.norm_size);
    case TYPE_FLOAT:
        return JSON_createFloatMatrix(file, FW.size, FW.norm_size);
    case TYPE_DOUBLE:
        return JSON_createDoubleMatrix(file, FW.size, FW.norm_size);

    default:
        return NULL;
    }
}

void *JSON_createIntMatrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    char c;
    int *matrix = malloc(norm_size * norm_size * sizeof(int));
    int row = 0, col = 0;
    char *token;

    rewind(file);

    while ((c = fgetc(file)) != EOF && c != '['); // Matrix start

    for (int i = 0; i < size; i++){
        while ((c = fgetc(file)) != EOF && c != '['); // Column start
        for (int j = 0; j < size; j++){
            token = readNextToken(file);
            if (token)
            {
                matrix[i * norm_size + j] = tokenToInt(token);
            }
        }
        for (int j = size; j < norm_size; j++){
            matrix[i * norm_size + j] = INT_MAX;
        }
    }

    for (int i = size; i < norm_size; i++)
    {
        for (int j = 0; j < norm_size; j++)
        {
            matrix[i * norm_size + j] = INT_MAX;
        }
    }

    return (void *)matrix;
}

void *JSON_createFloatMatrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    char c;
    float *matrix = malloc(norm_size * norm_size * sizeof(int));
    int row = 0, col = 0;
    char *token;

    rewind(file);

    while ((c = fgetc(file)) != EOF && c != '['); // Matrix start

    for (int i = 0; i < size; i++){
        while ((c = fgetc(file)) != EOF && c != '['); // Column start
        for (int j = 0; j < size; j++){
            token = readNextToken(file);
            if (token)
            {
                matrix[i * norm_size + j] = tokenToFloat(token);
            }
        }
        for (int j = size; j < norm_size; j++){
            matrix[i * norm_size + j] = FLT_MAX;
        }
    }

    for (int i = size; i < norm_size; i++)
    {
        for (int j = 0; j < norm_size; j++)
        {
            matrix[i * norm_size + j] = FLT_MAX;
        }
    }

    return (void *)matrix;
}


void *JSON_createDoubleMatrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    char c;
    double *matrix = malloc(norm_size * norm_size * sizeof(int));
    int row = 0, col = 0;
    char *token;

    rewind(file);

    while ((c = fgetc(file)) != EOF && c != '['); // Matrix start

    for (int i = 0; i < size; i++){
        while ((c = fgetc(file)) != EOF && c != '['); // Column start
        for (int j = 0; j < size; j++){
            token = readNextToken(file);
            if (token)
            {
                matrix[i * norm_size + j] = tokenToDouble(token);
            }
        }
        for (int j = size; j < norm_size; j++){
            matrix[i * norm_size + j] = DBL_MAX;
        }
    }

    for (int i = size; i < norm_size; i++)
    {
        for (int j = 0; j < norm_size; j++)
        {
            matrix[i * norm_size + j] = DBL_MAX;
        }
    }

    return (void *)matrix;
}