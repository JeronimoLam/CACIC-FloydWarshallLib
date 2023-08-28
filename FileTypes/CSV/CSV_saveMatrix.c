#include <stdio.h>
#include "CSV_Utils.h"
#include "../../Floyd-Warshall/FW_Lib.h"

static void saveIntMatrixToCSV(void * matrix, char * path, int size, int rows, int cols, char * filename);
static void saveFloatMatrixToCSV(void * matrix, char * path, int size, int rows, int cols, char * filename);
static void saveDoubleMatrixToCSV(void * matrix, char * path, int size, int rows, int cols, char * filename);

void CSV_saveMatrix(FW_Matrix * FW, char * path, int dist_matrix, int path_matrix) {
    switch(FW->datatype) {
        case TYPE_INT:
            if (dist_matrix) {
                saveIntMatrixToCSV(FW->dist, path,FW->norm_size, FW->rows, FW->cols, "distancias.csv");
            }
            if (path_matrix) {
                saveIntMatrixToCSV(FW->path, path, FW->norm_size, FW->rows, FW->cols, "caminos.csv");
            }
            break;
        case TYPE_FLOAT:
            if (dist_matrix) {
                saveFloatMatrixToCSV(FW->dist, path, FW->norm_size, FW->rows, FW->cols, "distancias.csv");
            }
            if (path_matrix) {
                saveFloatMatrixToCSV(FW->path, path, FW->norm_size, FW->rows, FW->cols, "caminos.csv");
            }
            break;
        case TYPE_DOUBLE:
            if (dist_matrix) {
                saveDoubleMatrixToCSV(FW->dist, path, FW->norm_size, FW->rows, FW->cols, "distancias.csv");
            }
            if (path_matrix) {
                saveDoubleMatrixToCSV(FW->path, path, FW->norm_size, FW->rows, FW->cols, "caminos.csv");
            }
            break;
        default:
            return;
    }
}
static void saveIntMatrixToCSV(void * matrix, char * path, int size, int rows, int cols, char * filename){
    int * matrix_int = (int *)matrix;  // Casting void* to int*

    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s/%s", path, filename);  // Concatenate path and filename

    FILE *file = fopen(fullPath, "w");  // Use the full path here
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            fprintf(file, "%d", matrix_int[row * size + col]);
            if (col < cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveFloatMatrixToCSV(void * matrix, char * path, int size, int rows, int cols, char * filename){
    float * matrix_float = (float *)matrix;  // Casting void* to float*

    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s/%s", path, filename);  // Concatenate path and filename

    FILE *file = fopen(fullPath, "w");  // Use the full path here
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            fprintf(file, "%.4f", matrix_float[row * size + col]);  // You can adjust the number of decimals
            if (col < cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

static void saveDoubleMatrixToCSV(void * matrix, char * path, int size, int rows, int cols, char * filename){
    double * matrix_double = (double *)matrix;  // Casting void* to double*

    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s/%s", path, filename);  // Concatenate path and filename

    FILE *file = fopen(fullPath, "w");  // Use the full path here
    if (file == NULL) {
        printf("Could not open file for saving.\n");
        return;
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            fprintf(file, "%.8f", matrix_double[row * size + col]);  // You can adjust the number of decimals
            if (col < cols - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}