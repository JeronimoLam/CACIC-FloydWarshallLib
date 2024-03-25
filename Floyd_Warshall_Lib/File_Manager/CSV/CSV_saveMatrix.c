#include <stdio.h>
#include "CSV_Utils.h"

static void saveIntMatrixToCSV(void *, char *, unsigned int, unsigned int, char *, unsigned int);
static void saveFloatMatrixToCSV(void *, char *, unsigned int, unsigned int, char *, unsigned int);
static void saveDoubleMatrixToCSV(void *, char *, unsigned int, unsigned int, char *, unsigned int);

void CSV_saveMatrix(FW_Matrix FW, char *path, int dist_matrix, int path_matrix)
{
    switch (FW.datatype)
    {
    case TYPE_INT:

        if (dist_matrix)
        {
            saveIntMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "distancias.csv", FW.BS);
        }
        if (path_matrix)
        {
            saveIntMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "caminos.csv", FW.BS);
        }
        break;
    case TYPE_FLOAT:
        if (dist_matrix)
        {
            saveFloatMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "distancias.csv", FW.BS);
        }
        if (path_matrix)
        {
            saveFloatMatrixToCSV(FW.path, path, FW.norm_size, FW.size, "caminos.csv", FW.BS);
        }
        break;
    case TYPE_DOUBLE:
        if (dist_matrix)
        {
            saveDoubleMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "distancias.csv", FW.BS);
        }
        if (path_matrix)
        {
            saveDoubleMatrixToCSV(FW.path, path, FW.norm_size, FW.size, "caminos.csv", FW.BS);
        }
        break;
    default:
        return;
    }
}
static void saveIntMatrixToCSV(void *matrix, char *path, unsigned int norm_size, unsigned int size, char *extension, unsigned int BS)
{
    int *ordered_matrix_int = reorganizeToLinear(matrix, norm_size, BS, TYPE_INT);
    free(matrix); // Is not used anymore // TODO: Revisar si conviene liberar la memoria acá o en el main

    // Open the file
    char fullPath[1024]; // Buffer for full path
    sprintf(fullPath, "%s_%s", path, extension);
    // printf("%s\n", fullPath);

    FILE *file = fopen(fullPath, "w");
    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    // Write the matrix to the file
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            int value = ordered_matrix_int[row * norm_size + col];
            if (value == INT_MAX)
            {
                fprintf(file, "INF");
            }
            else
            {
                fprintf(file, "%d", value);
            }
            if (col < size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveFloatMatrixToCSV(void *matrix, char *path, unsigned int norm_size, unsigned int size, char *extension, unsigned int BS)
{

    float *ordered_matrix_float = reorganizeToLinear(matrix, norm_size, BS, TYPE_FLOAT);
    free(matrix); // Is not used anymore

    // Open the file
    char fullPath[1024];                         // Buffer for full path
    sprintf(fullPath, "%s_%s", path, extension); // Concatenate path and filename

    FILE *file = fopen(fullPath, "w"); // Use the full path here
    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    // Reorder the matrix from blocks to rows to be saved
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            float value = ordered_matrix_float[row * norm_size + col];
            if (value == FLT_MAX)
            {
                fprintf(file, "INF");
            }
            else
            {
                fprintf(file, "%.4f", ordered_matrix_float[row * norm_size + col]); // You can adjust the number of decimals
            }
            if (col < size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveDoubleMatrixToCSV(void *matrix, char *path, unsigned int norm_size, unsigned int size, char *extension, unsigned int BS)
{
    double *ordered_matrix_double = reorganizeToLinear(matrix, norm_size, BS, TYPE_INT);

    free(matrix); // Is not used anymore

    // Open the file
    char fullPath[1024];                         // Buffer for full path
    sprintf(fullPath, "%s_%s", path, extension); // Concatenate path and filename
    printf("%s\n", fullPath);
    FILE *file = fopen(fullPath, "w"); // Use the full path here
    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    // Write the matrix to the file
    for (int row = 0; row < size; ++row)
    {
        for (int col = 0; col < size; ++col)
        {
            double value = ordered_matrix_double[row * norm_size + col];
            if (value == DBL_MAX)
            {
                fprintf(file, "INF");
            }
            else
            {
                fprintf(file, "%.10f", ordered_matrix_double[row * norm_size + col]); // You can adjust the number of decimals
            }
            if (col < size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}