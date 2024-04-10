#include <float.h>

#include "CSV_Utils.h"

void * CSV_createIntMatrix(FILE *file, unsigned int size, unsigned int norm_size);
void * CSV_createFloatMatrix(FILE *file, unsigned int size, unsigned int norm_size);
void * CSV_createDoubleMatrix(FILE *, unsigned int, unsigned int);

void *CSV_createMatrix(FW_Matrix FW, FILE *file)
{
    switch (FW.datatype)
    {
    case TYPE_INT:
        return CSV_createIntMatrix(file, FW.size, FW.norm_size);
    case TYPE_FLOAT:
        return CSV_createFloatMatrix(file, FW.size, FW.norm_size);
    case TYPE_DOUBLE:
        return CSV_createDoubleMatrix(file, FW.size, FW.norm_size);

    default:
        return NULL;
    }
}

void * CSV_createIntMatrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    rewind(file); // Ensure we start reading from the beginning of the file

    int *matrix = (int *)malloc(norm_size * norm_size * sizeof(int));
    if (!matrix)
        exit(9); // Allocation failed

    unsigned int row = 0, col = 0;
    char *token;

    for (unsigned int i = 0; i < norm_size * norm_size; i++)
    {
        if (row < size && col < size)
        {
            // Only attempt to read new tokens if within the original matrix size
            token = readNextToken(file);
            if (token)
            {
                matrix[i] = tokenToInt(token);
                // free(token); // Free the token now that we're done with it
                if (++col == norm_size)
                {
                    col = 0;
                    row++;
                }
                continue;
            }
        }
        // For cells outside the original matrix or after file content ends, fill with INT_MAX
        matrix[i] = INT_MAX;
        if (++col == norm_size)
        {
            col = 0;
            row++;
        }
    }

    return matrix;
}

void * CSV_createFloatMatrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    rewind(file); // Asegúrate de comenzar la lectura desde el inicio del archivo

    // Cambia la asignación de memoria para almacenar floats
    float *matrix = (float *)malloc(norm_size * norm_size * sizeof(float));
    if (!matrix)
        exit(9); // Fallo en la asignación

    unsigned int row = 0, col = 0;
    char *token;

    for (unsigned int i = 0; i < norm_size * norm_size; i++)
    {
        if (row < size && col < size)
        {
            // Solo intenta leer nuevos tokens si está dentro del tamaño de la matriz original
            token = readNextToken(file);
            if (token)
            {
                matrix[i] = tokenToFloat(token);
                // free(token); // Libera el token ahora que hemos terminado con él
                if (++col == norm_size)
                {
                    col = 0;
                    row++;
                }
                continue;
            }
        }
        // Para las celdas fuera de la matriz original o después de que termina el contenido del archivo, rellena con FLT_MAX
        matrix[i] = FLT_MAX;
        if (++col == norm_size)
        {
            col = 0;
            row++;
        }
    }

    return matrix;
}

// Modificada para trabajar con doubles
void * CSV_createDoubleMatrix(FILE *file, unsigned int size, unsigned int norm_size)
{
    rewind(file); // Asegúrate de comenzar la lectura desde el inicio del archivo

    // Cambia la asignación de memoria para almacenar doubles
    double *matrix = (double *)malloc(norm_size * norm_size * sizeof(double));
    if (!matrix)
        exit(9); // Fallo en la asignación

    unsigned int row = 0, col = 0;
    char *token;

    for (unsigned int i = 0; i < norm_size * norm_size; i++)
    {
        if (row < size && col < size)
        {
            token = readNextToken(file);
            if (token)
            {
                matrix[i] = tokenToDouble(token); // Utiliza la nueva función tokenToDouble
                if (++col == norm_size)
                {
                    col = 0;
                    row++;
                }
                continue;
            }
        }
        // Rellena con DBL_MAX para celdas fuera de la matriz original o después de que termina el contenido del archivo
        matrix[i] = DBL_MAX;
        if (++col == norm_size)
        {
            col = 0;
            row++;
        }
    }

    return matrix;
}