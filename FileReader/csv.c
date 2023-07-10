#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
char* CSV_createMatrixChar(FILE* file, int size) {
    char *matrix = (char *)malloc(size * size * sizeof(char));

    char *line = NULL;
    size_t len = 0;
    char *token;
    int row = 0, col = 0;

    while ((getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);  // You need to ensure the trim() function is defined
            matrix[row * size + col] = token[0]; // Convert string to char and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    if (line)
        free(line);

    return matrix;
}

int* CSV_createMatrixInt(FILE* file, int size) {
    int *matrix = (int *)malloc(size * size * sizeof(int));

    char *line = NULL;
    size_t len = 0;
    char *token;
    int row = 0, col = 0;

    while ((getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            matrix[row * size + col] = atoi(token); // Convert string to int and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    if (line)
        free(line);

    return matrix;
}

float* CSV_createMatrixFloat(FILE* file, int size) {
    float *matrix = (float *)malloc(size * size * sizeof(float));

    char *line = NULL;
    size_t len = 0;
    char *token;
    int row = 0, col = 0;

    while ((getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            matrix[row * size + col] = atof(token); // Convert string to float and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    if (line)
        free(line);

    return matrix;
}

double* CSV_createMatrixDouble(FILE* file, int size) {
    double *matrix = (double *)malloc(size * size * sizeof(double));

    char *line = NULL;
    size_t len = 0;
    char *token;
    int row = 0, col = 0;

    while ((getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            matrix[row * size + col] = strtod(token, NULL); // Convert string to double and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    if (line)
        free(line);

    return matrix;
}
*/