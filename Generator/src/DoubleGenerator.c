#include "matrix_operations.h"

void generarMatrizDouble(int n, double ***matriz) {
    *matriz = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        (*matriz)[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            (*matriz)[i][j] = (double)rand() / (double)(RAND_MAX) * 100; // Genera un número doble aleatorio entre 0 y 100
        }
    }
}


void guardarMatrizCSVDouble(double **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el CSV.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.16lf", matriz[i][j]);
            if (j < n - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void guardarMatrizJSONDouble(double **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        exit(1);
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"tipo\": \"double\",\n");
    fprintf(file, "  \"dimension\": %d,\n", n);
    fprintf(file, "  \"matriz\": [\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "    [");
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.16lf", matriz[i][j]);
            if (j < n - 1) fprintf(file, ", ");
        }
        fprintf(file, "]");
        if (i < n - 1) fprintf(file, ",\n");
    }

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}

