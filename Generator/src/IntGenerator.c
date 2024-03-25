#include "matrix_operations.h"

void generarMatrizInt(int n, int ***matriz, int maxValue) {
    *matriz = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        (*matriz)[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            if (i == j)
                (*matriz)[i][j] = 0;
            else
                // Generates a random int between 0 and maxValue
                (*matriz)[i][j] = rand() % (maxValue + 1);
                
        }
    }
}

void guardarMatrizCSVInt(int **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el CSV.\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fprintf(file, "%d", matriz[i][j]);
            if (j < n - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void guardarMatrizJSONInt(int **matriz, int n, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("No se pudo abrir el archivo para escribir el JSON.\n");
        exit(1);
    }

    fprintf(file, "{\n");
    fprintf(file, "  \"tipo\": \"int\",\n");
    fprintf(file, "  \"dimension\": %d,\n", n);
    fprintf(file, "  \"matriz\": [\n");

    for (int i = 0; i < n; i++) {
        fprintf(file, "    [");
        for (int j = 0; j < n; j++) {
            fprintf(file, "%d", matriz[i][j]);
            if (j < n - 1) fprintf(file, ", ");
        }
        fprintf(file, "]");
        if (i < n - 1) fprintf(file, ",\n");
    }

    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");

    fclose(file);
}
