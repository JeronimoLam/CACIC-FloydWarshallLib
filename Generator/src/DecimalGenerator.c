#include "matrix_operations.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

void generateRandomNumber(char *number, int n);

void DoubleMatrixGenerator(int n, double ***matriz, int parteEntera, int parteDecimal, int decimalCero, double porcentajeNegativos)
{

    if (parteEntera + (decimalCero ? 0 : parteDecimal) > 16)
    {
        printf("Advertencia: Se está perdiendo precisión.\n");
    }

    *matriz = (double **)malloc(n * sizeof(double *));
    if (*matriz == NULL)
    {
        printf("No se pudo reservar memoria para la matriz.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++)
    {
        (*matriz)[i] = (double *)malloc(n * sizeof(double));
        if ((*matriz)[i] == NULL)
        {
            printf("No se pudo reservar memoria para la matriz.\n");
            exit(1);
        }
        for (int j = 0; j < n; j++)
        {
            if (i == j)
            {
                (*matriz)[i][j] = 0.0;
            }
            else
            {
                // Chequeo de -1 basado en el porcentaje dado
                if (((double)rand() / RAND_MAX) < porcentajeNegativos)
                {
                    (*matriz)[i][j] = -1.0;
                    continue; // Continúa con el siguiente elemento de la matriz
                }

                char *parteEnteraStr = (char *)malloc(parteEntera + 1);

                // Generación de la parte entera
                generateRandomNumber(parteEnteraStr, parteEntera);
                double parteEnteraValor = strtod(parteEnteraStr, NULL);

                // Generación de la parte decimal
                char *parteDecimalStr = (char *)malloc(parteDecimal + 1);
                double parteDecimalValor = 0.0;
                if (!decimalCero && parteDecimal > 0)
                {

                    parteDecimalStr[0] = '0';
                    parteDecimalStr[1] = '.';
                    generateRandomNumber(parteDecimalStr + 2, parteDecimal);
                    parteDecimalValor = strtod(parteDecimalStr, NULL);
                }

                // Combinación de las partes para formar el número
                (*matriz)[i][j] = parteEnteraValor + parteDecimalValor;

            }
        }
    }
}

void DoubleMatrix2CSV(double **matriz, int n, const char *path, int parteDecimal, int decimalCero)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo para escribir el CSV.\n");
        exit(1);
    }

    char formatString[20];
    // Asegura que las cifras decimales se muestren según parteDecimal, incluso si son cero
    sprintf(formatString, "%%.%dlf", parteDecimal);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fprintf(file, formatString, matriz[i][j]);
            if (j < n - 1)
                fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void DoubleMatrix2JSON(double **matriz, int n, const char *path, int parteDecimal, int decimalCero)
{
    FILE *file = fopen(path, "w");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        exit(1);
    }

    char formatString[20];
    // Asegura que las cifras decimales se muestren según parteDecimal, incluso si son cero
    sprintf(formatString, "%%.%dlf", parteDecimal);

    fprintf(file, "{\n");
    fprintf(file, "  \"tipo\": \"double\",\n");
    fprintf(file, "  \"dimension\": %d,\n", n);
    fprintf(file, "  \"matriz\": [\n");

    for (int i = 0; i < n; i++)
    {
        fprintf(file, "    [");
        for (int j = 0; j < n; j++)
        {
            fprintf(file, formatString, matriz[i][j]);
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

void generateRandomNumber(char *number, int n)
{
    for (int i = 0; i < n; i++)
    {
        int digit = rand() % 10; // Genera un dígito aleatorio entre 0 y 9
        number[i] = '0' + digit; // Convierte el dígito a su representación como carácter y lo almacena
    }
    number[n] = '\0'; // Asegura que la cadena esté correctamente terminada
}