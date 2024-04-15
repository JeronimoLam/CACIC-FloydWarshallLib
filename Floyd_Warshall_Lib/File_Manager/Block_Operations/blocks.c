
#include "blocks.h"

void *organize_to_blocks(void *matrix, unsigned int size, unsigned int BS, DataType type)
{
    if (size % BS != 0)
    {
        // Manejar el error o ajustar el tamaño de la matriz según sea necesario
        return NULL;
    }

    unsigned int r = size / BS;       // Número de bloques a lo largo de una dimensión
    unsigned int block_size = BS * BS; // Elementos en un bloque
    void *block_matrix = NULL;

    switch (type)
    {
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

    if (block_matrix == NULL)
    {
        fprintf(stderr, "Error: Allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED); // Allocation failed
    }
    double value;

    for (unsigned int I = 0; I < r; I++)
        for (unsigned int J = 0; J < r; J++)
            for (unsigned int i = 0; i < BS; i++)
                for (unsigned int j = 0; j < BS; j++)
                    switch (type)
                    {
                    case TYPE_INT:
                        ((int *)block_matrix)[I * size * BS + J * block_size + i * BS + j] =
                            ((int *)matrix)[I * size * BS + J * BS + i * size + j];
                        break;
                    case TYPE_FLOAT:
                        ((float *)block_matrix)[I * size * BS + J * block_size + i * BS + j] =
                            ((float *)matrix)[I * size * BS + J * BS + i * size + j];
                        break;
                    case TYPE_DOUBLE:
                        value = ((double *)matrix)[I * size * BS + J * BS + i * size + j];
                        ((double *)block_matrix)[I * size * BS + J * block_size + i * BS + j] = value;
                        break;
                    default:
                        // Este caso ya fue manejado antes
                        break;
                    }

    return block_matrix;
}

void *reorganize_to_linear(void *block_matrix, unsigned int size, unsigned int BS, DataType type)
{

    unsigned int r = size / BS; // Number of blocks along one dimension
    void *original_matrix = NULL;

    // Allocate memory based on data type
    switch (type)
    {
    case TYPE_INT:
        original_matrix = malloc(size * size * sizeof(int));
        break;
    case TYPE_FLOAT:
        original_matrix = malloc(size * size * sizeof(float));
        break;
    case TYPE_DOUBLE:
        original_matrix = malloc(size * size * sizeof(double));
        break;
    default:
        // Unsupported data type
        return NULL;
    }

    if (original_matrix == NULL)
    {
        fprintf(stderr, "Error: Allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED); // Allocation failed
    }

    // Copy data from the block format back to the original linear format
    for (unsigned int I = 0; I < r; I++)
        for (unsigned int J = 0; J < r; J++)
            for (unsigned int i = 0; i < BS; i++)
                for (unsigned int j = 0; j < BS; j++)
                    switch (type)
                    {
                    case TYPE_INT:
                        ((int *)original_matrix)[I * size * BS + J * BS + i * size + j] =
                            ((int *)block_matrix)[I * BS * BS * r + J * BS * BS + i * BS + j];
                        break;
                    case TYPE_FLOAT:
                        ((float *)original_matrix)[I * size * BS + J * BS + i * size + j] =
                            ((float *)block_matrix)[I * BS * BS * r + J * BS * BS + i * BS + j];
                        break;
                    case TYPE_DOUBLE:
                        ((double *)original_matrix)[I * size * BS + J * BS + i * size + j] =
                            ((double *)block_matrix)[I * BS * BS * r + J * BS * BS + i * BS + j];
                        break;
                    default:
                        // This case is already handled
                        break;
                    }

    return original_matrix;
}