#include <float.h>
#include "CSV_Utils.h"

static void* createIntMatrix(FILE*, unsigned int, unsigned int);
static void* createFloatMatrix(FILE*, unsigned int, unsigned int);
static void* createDoubleMatrix(FILE*, unsigned int, unsigned int);
static void* createCharMatrix(FILE*, unsigned int, unsigned int);

void* CSV_createMatrix(FW_Matrix FW, FILE* file) {
    switch(FW.datatype) {
        case TYPE_INT:
            return createIntMatrix(file, FW.norm_size, FW.BS);
        case TYPE_FLOAT:
            return createFloatMatrix(file,  FW.norm_size, FW.BS);
        case TYPE_DOUBLE:
            return createDoubleMatrix(file,  FW.norm_size, FW.BS);

        default:
            return NULL;
    }
}

// These functions are similar but each one uses atoi, atof, strtod, or a simple assignment, respectively.
static void* createIntMatrix(FILE* file, unsigned int size, unsigned int BS) {
    int *matrix = (int *)malloc(size * size * sizeof(int));
    char *token;
    int row = 0, col = 0;
    char line[2048];

    // Initialize while reading the file
    for (row = 0; row < size; ++row) {
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            token = strtok(line, ",");
            col = 0;
            while (token != NULL && col < size) {
                matrix[row * size + col] = atoi(token);
                token = strtok(NULL, ",");
                col++;
            }
        }
        // Fill remaining columns in the current row with INT_MAX
        for (; col < size; ++col) {
            matrix[row * size + col] = INT_MAX;
        }
        col = 0; // Reset column counter for the next row
    }

    return reorganizeToBlocks(matrix, size, BS, TYPE_INT);

    // //Reorder by blocks
    // int *block_matrix = (int *)malloc(size * size * sizeof(int));

    // unsigned int I,J,i,j,blockSize,r;
    // r = size / BS;
    // blockSize = BS * BS;
    // for(I=0; I<r; I++)
    //     for(J=0; J<r; J++)
    //         for(i=0; i < BS; i++)
    //             for(j=0; j < BS; j++)
    //                 block_matrix[I * size * BS + J * blockSize + i *BS + j] = matrix[I * size * BS + J * BS + i * size + j];

    // return block_matrix;
}
static void* createFloatMatrix(FILE* file, unsigned int size, unsigned int BS) {
    float *matrix = (float *)malloc(size * size * sizeof(float));
    char *token;
    int row = 0, col = 0;
    char line[2048];

    for (row = 0; row < size; ++row) {
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            token = strtok(line, ",");
            col = 0;
            while (token != NULL && col < size) {
                matrix[row * size + col] = atof(token);
                token = strtok(NULL, ",");
                col++;
            }
        }
        // Fill remaining columns in the current row with FLOAT_MAX
        for (; col < size; ++col) {
            matrix[row * size + col] = FLT_MAX;
        }
        col = 0; // Reset column counter for the next row
    }

    return reorganizeToBlocks(matrix, size, BS, TYPE_FLOAT);
    // //Reorder by blocks
    // float *block_matrix = (float *)malloc(size * size * sizeof(float));

    // unsigned int I,J,i,j,blockSize,r;
    // r = size / BS;
    // blockSize = BS * BS;
    // for(I=0; I<r; I++)
    //     for(J=0; J<r; J++)
    //         for(i=0; i < BS; i++)
    //             for(j=0; j < BS; j++)
    //                 block_matrix[I * size * BS + J * blockSize + i *BS + j] = matrix[I * size * BS + J * BS + i * size + j];


    // return block_matrix;
}

static void* createDoubleMatrix(FILE* file, unsigned int size, unsigned int BS) {
    double *matrix = (double *)malloc(size * size * sizeof(double));
    char *token;
    int row = 0, col = 0;
    char line[2048];

    for (row = 0; row < size; ++row) {
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0';
            token = strtok(line, ",");
            col = 0;
            while (token != NULL && col < size) {
                matrix[row * size + col] = strtod(token, NULL);
                token = strtok(NULL, ",");
                col++;
            }
        }
        // Fill remaining columns in the current row with DBL_MAX
        for (; col < size; ++col) {
            matrix[row * size + col] = DBL_MAX;
        }
        col = 0; // Reset column counter for the next row
    }

    return reorganizeToBlocks(matrix, size, BS, TYPE_DOUBLE);
    // //Reorder by blocks
    // double *block_matrix = (double *)malloc(size * size * sizeof(double));

    // unsigned int I,J,i,j,blockSize,r;
    // r = size / BS;
    // blockSize = BS * BS;
    // for(I=0; I<r; I++)
    //     for(J=0; J<r; J++)
    //         for(i=0; i < BS; i++)
    //             for(j=0; j < BS; j++)
    //                 block_matrix[I * size * BS + J * blockSize + i *BS + j] = matrix[I * size * BS + J * BS + i * size + j];

    // return block_matrix;
}