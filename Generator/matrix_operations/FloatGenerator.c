#include "matrix_operations.h"

void generarMatrizFloat(int n, float ***matriz) {
    *matriz = (float **)malloc(n * sizeof(float *));
    for (int i = 0; i < n; i++) {
        (*matriz)[i] = (float *)malloc(n * sizeof(float));
        for (int j = 0; j < n; j++) {
            (*matriz)[i][j] = (float)rand() / (float)(RAND_MAX) * 100.0; // Genera un número flotante aleatorio entre 0 y 100
        }
    }
}

void guardarMatrizCSVFloat(float **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el CSV.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.2f", matriz[i][j]);
            if (j < n - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void guardarMatrizJSONFloat(float **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        return;
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"tipo\": \"float\",\n");
    fprintf(file, "  \"dimension\": %d,\n", n);
    fprintf(file, "  \"matriz\": [\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "    [");
        for (int j = 0; j < n; j++) {
            fprintf(file, "%.2f", matriz[i][j]);
            if (j < n - 1) fprintf(file, ", ");
        }
        fprintf(file, "]");
        if (i < n - 1) fprintf(file, ",\n");
    }

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}
