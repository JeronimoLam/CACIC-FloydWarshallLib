#include "JSON_Utils.h"

void *JSON_create_int_matrix(FILE *file, unsigned int size, unsigned int norm_size);
void *JSON_create_float_matrix(FILE *file, unsigned int size, unsigned int norm_size);
void *JSON_create_double_matrix(FILE *file, unsigned int size, unsigned int norm_size);

void *create_json_matrix(FW_Matrix FW, FILE *file)
{
    switch (FW.datatype)
    {
    case TYPE_INT:
        return JSON_create_int_matrix(file, FW.size, FW.norm_size);
    case TYPE_FLOAT:
        return JSON_create_float_matrix(file, FW.size, FW.norm_size);
    case TYPE_DOUBLE:
        return JSON_create_double_matrix(file, FW.size, FW.norm_size);

    default:
        return NULL;
    }
}

void *JSON_create_int_matrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    char c,  *token;

    int *matrix = malloc(norm_size * norm_size * sizeof(int));
    if (!matrix)
    {
        fprintf(stderr, "Error: Allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED); // Allocation failed
    }
    

    while ((c = fgetc(file)) != EOF && c != '['); // Matrix start

    for (int i = 0; i < size; i++){
        while ((c = fgetc(file)) != EOF && c != '['); // Column start
        for (int j = 0; j < size; j++){
            token = read_next_token(file);
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

void *JSON_create_float_matrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    char c, *token;
    float *matrix = malloc(norm_size * norm_size * sizeof(float));

    while ((c = fgetc(file)) != EOF && c != '['); // Matrix start

    for (int i = 0; i < size; i++){
        while ((c = fgetc(file)) != EOF && c != '['); // Column start
        for (int j = 0; j < size; j++){
            token = read_next_token(file);
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


void *JSON_create_double_matrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    char c, * token;
    double *matrix = malloc(norm_size * norm_size * sizeof(double));

    while ((c = fgetc(file)) != EOF && c != '['); // Matrix start

    for (int i = 0; i < size; i++){
        while ((c = fgetc(file)) != EOF && c != '['); // Column start
        for (int j = 0; j < size; j++){
            token = read_next_token(file);
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