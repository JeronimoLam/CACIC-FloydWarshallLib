#include "CSV_Utils.h"

static void* createIntMatrix(FILE*, int, int);
static void* createFloatMatrix(FILE*, int, int);
static void* createDoubleMatrix(FILE*, int, int);
static void* createCharMatrix(FILE*, int, int);

void* CSV_createMatrix(FILE* file, int cols, int rows, DataType dataType) {
    switch(dataType) {
        case TYPE_INT:
            return createIntMatrix(file, cols, rows);
        case TYPE_FLOAT:
            return createFloatMatrix(file,  cols, rows);
        case TYPE_DOUBLE:
            return createDoubleMatrix(file,  cols, rows);
        case TYPE_CHAR:
            return createCharMatrix(file,  cols, rows);
        default:
            return NULL;
    }
}

// These functions are similar but each one uses atoi, atof, strtod, or a simple assignment, respectively.
static void* createIntMatrix(FILE* file, int cols, int rows) {
    int *matrix = (int *)malloc(cols * cols * sizeof(int));

    char *token;
    int row = 0, col = 0;

    char line[2048]; // Size correct for the app?

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            matrix[row * cols + col] = atoi(token); // Convert string to int and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }
    return matrix;
}

static void* createFloatMatrix(FILE* file, int cols, int rows) {
    float *matrix = (float *)malloc(cols * cols * sizeof(float));

    char *token;
    int row = 0, col = 0;

    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character at end of line if it exists
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            matrix[row * cols + col] = atof(token); // Convert string to float and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    return matrix;
}

static void* createDoubleMatrix(FILE* file, int cols, int rows) {
    double *matrix = (double *)malloc(cols * cols * sizeof(double));

    char *token;
    int row = 0, col = 0;

    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            matrix[row * cols + col] = strtod(token, NULL); // Convert string to double and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    return matrix;
}

static void* createCharMatrix(FILE* file, int cols, int rows) {
    char *matrix = (char *)malloc(cols * cols * sizeof(char));

    char *token;
    int row = 0, col = 0;

    char line[2048];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);  // You need to ensure the trim() function is defined
            matrix[row * cols + col] = token[0]; // Convert string to char and store in matrix
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    return matrix;
}
