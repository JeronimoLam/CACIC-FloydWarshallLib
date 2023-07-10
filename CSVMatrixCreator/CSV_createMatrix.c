#include "CSV_createMatrix.h"

static void* createIntMatrix(FILE* file, int size);
static void* createFloatMatrix(FILE* file, int size);
static void* createDoubleMatrix(FILE* file, int size);
static void* createCharMatrix(FILE* file, int size);

void* CSV_createMatrix(FILE* file, int size, DataType dataType) {
    switch(dataType) {
        case TYPE_INT:
            return createIntMatrix(file, size);
        case TYPE_FLOAT:
            return createFloatMatrix(file, size);
        case TYPE_DOUBLE:
            return createDoubleMatrix(file, size);
        case TYPE_CHAR:
            return createCharMatrix(file, size);
        default:
            return NULL;
    }
}

// You need to define these functions. They are similar to the ones I've shown before,
// but each one uses atoi, atof, strtod, or a simple assignment, respectively.
static void* createIntMatrix(FILE* file, int size) {
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

static void* createFloatMatrix(FILE* file, int size) {
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

static void* createDoubleMatrix(FILE* file, int size) {
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

static void* createCharMatrix(FILE* file, int size) {
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
