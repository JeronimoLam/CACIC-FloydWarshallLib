#include "matrix_operations.h"

void generarMatrizInt(int n, int ***matriz) {
    *matriz = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        (*matriz)[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            (*matriz)[i][j] = rand() % 101; // Genera un número aleatorio entre 0 y 100
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
