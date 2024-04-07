#include "matrix_operations.h"

void IntMatrixGenerator(int n, int ***matriz, int maxValue, double porcentajeNegativos, int direction)
{
    *matriz = (int **)malloc(n * sizeof(int *));
    if (*matriz == NULL)
    {
        printf("No se pudo reservar memoria para la matriz.\n");
        exit(1);
    }
    int value;
    for (int i = 0; i < n; i++)
    {
        (*matriz)[i] = (int *)malloc(n * sizeof(int));
        if ((*matriz)[i] == NULL)
        {
            printf("No se pudo reservar memoria para la matriz.\n");
            exit(1);
        }
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                (*matriz)[i][j] = 0;
            else if (!direction && i < j)
                continue;
            else
            {

                // Chequeo de -1 basado en el porcentaje dado
                if (((double)rand() / RAND_MAX) < porcentajeNegativos)
                {
                    (*matriz)[i][j] = -1;
                    if (!direction)
                        (*matriz)[j][i] = -1;
                    continue; // Continúa con el siguiente elemento de la matriz
                }

                // Generates a random int between 0 and maxValue
                value = (rand() % (maxValue + 1));
                if (value == 0)
                    value = -1;

                (*matriz)[i][j] = value;
                if (!direction)
                    (*matriz)[j][i] = value;
            }
        }
    }
}

void IntMatrix2CSV(int **matriz, int n, const char *path)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo para escribir el CSV.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(file, "%d", matriz[i][j]);
            if (j < n - 1)
                fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void IntMatrix2JSON(int **matriz, int n, const char *path)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        exit(1);
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"tipo\": \"int\",\n");
    fprintf(file, "  \"dimension\": %d,\n", n);
    fprintf(file, "  \"matriz\": [\n");

    for (int i = 0; i < n; i++)
    {
        fprintf(file, "    [");
        for (int j = 0; j < n; j++)
        {
            fprintf(file, "%d", matriz[i][j]);
            if (j < n - 1)
                fprintf(file, ", ");
        }
        fprintf(file, "]");
        if (i < n - 1)
            fprintf(file, ",\n");
    }

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}
