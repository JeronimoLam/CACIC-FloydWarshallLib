#include <stdio.h>
#include "CSV_Utils.h"

static void saveIntMatrixToCSV(FW_Matrix *, int *, char *, char *, unsigned int);
static void saveFloatMatrixToCSV(FW_Matrix *, char *, char *, unsigned int);
static void saveDoubleMatrixToCSV(FW_Matrix *, char *, char *, unsigned int);

void CSV_saveMatrix(FW_Matrix FW, char *path, unsigned int print_dist, unsigned int print_path, unsigned int disconnected_str)
{
    switch (FW.datatype)
    {
    case TYPE_INT:

        if (print_dist)
        {
            saveIntMatrixToCSV(&FW, (int *)FW.dist, path, "distances.csv", disconnected_str);
        }
        break;
    case TYPE_FLOAT:
        if (print_dist)
        {
            saveFloatMatrixToCSV(&FW, path, "distances.csv", disconnected_str);
        }
        break;
    case TYPE_DOUBLE:
        if (print_dist)
        {
            saveDoubleMatrixToCSV(&FW, path, "distances.csv", disconnected_str);
        }
        break;
    default:
        return;
    }
    if (print_path)
    {
        saveIntMatrixToCSV(&FW, FW.path, path, "path.csv", disconnected_str);
    }
}
static void saveIntMatrixToCSV(FW_Matrix *FW, int *matrix, char *path, char *extension, unsigned int disconnected_str)
{
    int *ordered_matrix_int = reorganizeToLinear(matrix, FW->norm_size, FW->BS, TYPE_INT);
    // free(matrix); // Is not used anymore // TODO: Revisar si conviene liberar la memoria acá o en el main

    // Abrir el archivo
    char fullPath[1024]; // Buffer para la ruta completa
    sprintf(fullPath, "%s_%s", path, extension);

    // printf("%s\n", fullPath);

    FILE *file = fopen(fullPath, "w");
    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    // Write the matrix to the file
    for (int row = 0; row < FW->size; ++row)
    {
        for (int col = 0; col < FW->size; ++col)
        {
            int value = ordered_matrix_int[row * FW->norm_size + col];
            if (value == INT_MAX && disconnected_str)
            {
                fprintf(file, "INF");
            }
            else
            {
                fprintf(file, "%d", value == INT_MAX ? -1 : value);
            }
            if (col < FW->size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveFloatMatrixToCSV(FW_Matrix *FW, char *path, char *extension, unsigned int disconnected_str)
{
    // Ajustar para obtener una matriz de floats reorganizada
    float *ordered_matrix_float = reorganizeToLinear(FW->dist, FW->norm_size, FW->BS, TYPE_FLOAT);

    // Abrir el archivo
    char fullPath[1024]; // Buffer para la ruta completa
    sprintf(fullPath, "%s_%s", path, extension);

    FILE *file = fopen(fullPath, "w");
    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    // Escribir la matriz en el archivo
    for (unsigned int row = 0; row < FW->size; ++row)
    {
        for (unsigned int col = 0; col < FW->size; ++col)
        {
            float value = ordered_matrix_float[row * FW->norm_size + col];
            if (value == FLT_MAX && disconnected_str)
            {
                fprintf(file, "INF");
            }
            else
            {
                fprintf(file, "%.*f", FW->decimal_length, value == FLT_MAX ? -1.0 : value);
            }

            if (col < FW->size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveDoubleMatrixToCSV(FW_Matrix *FW, char *path, char *extension, unsigned int disconnected_str)
{

    // Ajustar para obtener una matriz de doubles reorganizada
    double *ordered_matrix_double = reorganizeToLinear(FW->dist, FW->norm_size, FW->BS, TYPE_DOUBLE);

    // Abrir el archivo
    char fullPath[1024]; // Buffer para la ruta completa
    sprintf(fullPath, "%s_%s", path, extension);

    FILE *file = fopen(fullPath, "w");
    if (file == NULL)
    {
        printf("Could not open file for saving.\n");
        return;
    }

    // Escribir la matriz en el archivo
    for (unsigned int row = 0; row < FW->size; ++row)
    {
        for (unsigned int col = 0; col < FW->size; ++col)
        {
            double value = ordered_matrix_double[row * FW->norm_size + col];
            if (value == DBL_MAX && disconnected_str)
            {
                fprintf(file, "INF");
            }
            else
            {
                fprintf(file, "%.*lf", FW->decimal_length, value == DBL_MAX ? -1.0 : value);
            }
            if (col < FW->size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}