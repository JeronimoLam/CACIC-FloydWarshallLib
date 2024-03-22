#include <stdio.h>
#include "CSV_Utils.h"

static void saveIntMatrixToCSV(void * , char * , unsigned int ,unsigned int , char * , unsigned int);
static void saveFloatMatrixToCSV(void * , char * , unsigned int , unsigned int , char * , unsigned int);
static void saveDoubleMatrixToCSV(void * , char * , unsigned int , unsigned int , char * , unsigned int);

void CSV_saveMatrix(FW_Matrix FW, char * path, int dist_matrix, int path_matrix) {
    switch(FW.datatype) {
        case TYPE_INT:

            if (dist_matrix) {
                saveIntMatrixToCSV(FW.dist, path,FW.norm_size, FW.size, "distancias.csv", FW.BS);
            }
            if (path_matrix) {
                saveIntMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "caminos.csv", FW.BS);
            }
            break;
        case TYPE_FLOAT:
            if (dist_matrix) {
                saveFloatMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "distancias.csv", FW.BS);
            }
            if (path_matrix) {
                saveFloatMatrixToCSV(FW.path, path, FW.norm_size, FW.size, "caminos.csv", FW.BS);
            }
            break;
        case TYPE_DOUBLE:
            if (dist_matrix) {
                saveDoubleMatrixToCSV(FW.dist, path, FW.norm_size, FW.size, "distancias.csv", FW.BS);
            }
            if (path_matrix) {
                saveDoubleMatrixToCSV(FW.path, path, FW.norm_size, FW.size, "caminos.csv", FW.BS);
            }
            break;
        default:
            return;
    }
}
static void saveIntMatrixToCSV(void * matrix, char * path, unsigned int norm_size, unsigned int size, char * extension, unsigned int BS){
    // int * matrix_int = (int *)matrix;  // Casting void* to int*

    // // Reorder the matrix from blocks to rows to be saved
    // int * ordered_matrix_int = malloc(norm_size * norm_size * sizeof(int));
    // unsigned int I,J,i,j,blockSize,r;
    // r = norm_size / BS;

    // blockSize = BS * BS;
    // for(I=0; I<r; I++)
    //     for(J=0; J<r; J++)
    //         for(i=0; i < BS; i++)
    //             for(j=0; j < BS; j++)
    //                 ordered_matrix_int[I * norm_size * BS + J * BS + i * norm_size + j] = matrix_int[I * norm_size * BS + J * blockSize + i * BS + j];
    int * ordered_matrix_int = reorganizeToLinear(matrix, norm_size, BS, TYPE_INT);
    free(matrix); // Is not used anymore

    // Open the file
    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s_%s", path, extension);
    printf("%s\n", fullPath);


    FILE *file = fopen(fullPath, "w");
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    // Write the matrix to the file
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            fprintf(file, "%d", ordered_matrix_int[row * norm_size + col]);
            if (col < size - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveFloatMatrixToCSV(void * matrix, char * path, unsigned int norm_size, unsigned int size, char * extension, unsigned int BS){
    // float * matrix_float = (float *)matrix;  // Casting void* to float*

    // // Reorder the matrix from blocks to rows to be saved
    // float * ordered_matrix_float = malloc(norm_size * norm_size * sizeof(float));
    // unsigned int I,J,i,j,blockSize,r;
    // r = norm_size / BS;

    // blockSize = BS * BS;
    // for(I=0; I<r; I++)
    //     for(J=0; J<r; J++)
    //         for(i=0; i < BS; i++)
    //             for(j=0; j < BS; j++)
    //                 ordered_matrix_float[I * norm_size * BS + J * BS + i * norm_size + j] = matrix_float[I * norm_size * BS + J * blockSize + i * BS + j];
    
    float * ordered_matrix_float = reorganizeToLinear(matrix, norm_size, BS, TYPE_FLOAT);
    free(matrix); // Is not used anymore

    // Open the file
    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s_%s", path, extension);  // Concatenate path and filename

    FILE *file = fopen(fullPath, "w");  // Use the full path here
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    // Reorder the matrix from blocks to rows to be saved
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            fprintf(file, "%.4f", ordered_matrix_float[row * norm_size + col]);  // You can adjust the number of decimals
            if (col < size - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveDoubleMatrixToCSV(void * matrix, char * path, unsigned int norm_size, unsigned int size, char * extension, unsigned int BS){
    // double * matrix_double = (double *)matrix;  // Casting void* to double*

    // // Reorder the matrix to be saved
    // double * ordered_matrix_double = malloc(norm_size * norm_size * sizeof(double));
    // unsigned int I,J,i,j,blockSize,r;
    // r = norm_size / BS;

    // blockSize = BS * BS;
    // for(I=0; I<r; I++)
    //     for(J=0; J<r; J++)
    //         for(i=0; i < BS; i++)
    //             for(j=0; j < BS; j++)
    //                 ordered_matrix_double[I * norm_size * BS + J * BS + i * norm_size + j] = matrix_double[I * norm_size * BS + J * blockSize + i * BS + j];

    double * ordered_matrix_double = reorganizeToLinear(matrix, norm_size, BS, TYPE_INT);

    free(matrix); // Is not used anymore

    // Open the file
    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s_%s", path, extension);  // Concatenate path and filename
    printf("%s\n", fullPath);
    FILE *file = fopen(fullPath, "w");  // Use the full path here
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    // Write the matrix to the file
    for (int row = 0; row < size; ++row) {
        for (int col = 0; col < size; ++col) {
            fprintf(file, "%.8f", ordered_matrix_double[row * norm_size + col]);  // You can adjust the number of decimals
            if (col < size - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}