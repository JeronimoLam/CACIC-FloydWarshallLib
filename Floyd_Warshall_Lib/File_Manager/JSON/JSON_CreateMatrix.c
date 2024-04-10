#include "JSON_Utils.h"

void *JSON_createIntMatrix(FILE *file, unsigned int size, unsigned int norm_size);

//TODO: Implement Function for all types of data
int *JSON_createMatrix(FW_Matrix FW, FILE * file) {
    // Implement Function
    return NULL;
}

// Implementa la funcion que lee la matriz desde el JSON caracter por caracter.
// Ejemplo {
//              "type": "int",
//              "size": 3,
//              "matrix": [[1,2,3],
//                          [4,5,6],
//                          [7,8,9]]
//        } 
void *JSON_createIntMatrix(FILE *file, unsigned int size, unsigned int norm_size) {
    char c;
    int *matrix = malloc(norm_size * norm_size * sizeof(int));
    int row = 0, col = 0;
    int matrix_start = 0;
    
    // Buscar el inicio de la matriz
    while ((c = fgetc(file)) != EOF) {
        if (c == '[') {
            matrix_start = ftell(file);
            break;
        }
    }
    
    // Regresar al inicio de la matriz
    fseek(file, matrix_start, SEEK_SET);
    
    // Leer caracter por caracter y construir la matriz
    while ((c = fgetc(file)) != EOF) {
        if (c == '[') {
            col = 0;
        } else if (c == ']') {
            row++;
            if (row == size) {
                break;
            }
        } else if (c == ',' || c == ' ') {
            continue;
        } else if (c >= '0' && c <= '9') {
            matrix[row * norm_size + col] = c - '0';
            col++;
        }
    }
    
    // TODO: Hacer algo con la matriz
    
    return NULL;
}