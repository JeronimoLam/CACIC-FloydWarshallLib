
#include "blocks.h"

void * reorganizeToBlocks(void *matrix, unsigned int size, unsigned int BS, DataType type) {
    if (size % BS != 0) {
        // Manejar el error o ajustar el tamaño de la matriz según sea necesario
        return NULL;
    }

    unsigned int r = size / BS; // Número de bloques a lo largo de una dimensión
    unsigned int blockSize = BS * BS; // Elementos en un bloque
    void *block_matrix = NULL;

    switch (type) {
        case TYPE_INT:
            block_matrix = malloc(size * size * sizeof(int));
            break;
        case TYPE_FLOAT:
            block_matrix = malloc(size * size * sizeof(float));
            break;
        case TYPE_DOUBLE:
            block_matrix = malloc(size * size * sizeof(double));
            break;
        default:
            // Manejar tipos no definidos
            return NULL;
    }

    if (block_matrix == NULL) {
        // Manejo de error en caso de que malloc falle
        return NULL;
    }

    for (unsigned int I = 0; I < r; I++) 
        for (unsigned int J = 0; J < r; J++) 
            for (unsigned int i = 0; i < BS; i++) 
                for (unsigned int j = 0; j < BS; j++) 
                    switch (type) {
                        case TYPE_INT:
                            ((int*)block_matrix)[I * size * BS + J * blockSize + i * BS + j] = 
                                ((int*)matrix)[I * size * BS + J * BS + i * size + j];
                            break;
                        case TYPE_FLOAT:
                            ((float*)block_matrix)[I * size * BS + J * blockSize + i * BS + j] = 
                                ((float*)matrix)[I * size * BS + J * BS + i * size + j];
                            break;
                        case TYPE_DOUBLE:
                            ((double*)block_matrix)[I * size * BS + J * blockSize + i * BS + j] = 
                                ((double*)matrix)[I * size * BS + J * BS + i * size + j];
                            break;
                        default:
                            // Este caso ya fue manejado antes
                            break;
                    }
                
    return block_matrix;
}

void *reorganizeToLinear(void *blockMatrix, unsigned int size, unsigned int BS, DataType type) {

    unsigned int r = size / BS; // Number of blocks along one dimension
    void *originalMatrix = NULL;

    // Allocate memory based on data type
    switch (type) {
        case TYPE_INT:
            originalMatrix = malloc(size * size * sizeof(int));
            break;
        case TYPE_FLOAT:
            originalMatrix = malloc(size * size * sizeof(float));
            break;
        case TYPE_DOUBLE:
            originalMatrix = malloc(size * size * sizeof(double));
            break;
        default:
            // Unsupported data type
            return NULL;
    }

    if (originalMatrix == NULL) {
        return NULL;
    }

    // Copy data from the block format back to the original linear format
    for (unsigned int I = 0; I < r; I++) 
        for (unsigned int J = 0; J < r; J++) 
            for (unsigned int i = 0; i < BS; i++) 
                for (unsigned int j = 0; j < BS; j++)
                    switch (type) {
                        case TYPE_INT:
                            ((int*)originalMatrix)[I * size * BS + J * BS + i * size + j] =
                                ((int*)blockMatrix)[I * BS * BS * r + J * BS * BS + i * BS + j];
                            break;
                        case TYPE_FLOAT:
                            ((float*)originalMatrix)[I * size * BS + J * BS + i * size + j] =
                                ((float*)blockMatrix)[I * BS * BS * r + J * BS * BS + i * BS + j];
                            break;
                        case TYPE_DOUBLE:
                            ((double*)originalMatrix)[I * size * BS + J * BS + i * size + j] =
                                ((double*)blockMatrix)[I * BS * BS * r + J * BS * BS + i * BS + j];
                            break;
                        default:
                            // This case is already handled
                            break;
                    }

    return originalMatrix;
}