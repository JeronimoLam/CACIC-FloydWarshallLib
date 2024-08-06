
#include "blocks.h"

#if defined(_WIN32) || defined(_WIN64)
#include <malloc.h> // Para _aligned_malloc y _aligned_free
#endif

void *organize_to_blocks(void *matrix, unsigned int size, DataType type)
{
    if (size % BLOCK_SIZE != 0)
    {
        // Manejar el error o ajustar el tamaño de la matriz según sea necesario
        return NULL;
    }

    unsigned int r = size / BLOCK_SIZE;                // Número de bloques a lo largo de una dimensión
    unsigned int block_size = BLOCK_SIZE * BLOCK_SIZE; // Elementos en un bloque
    void *block_matrix = NULL;

#if defined(_WIN32) || defined(_WIN64)
    switch (type)
    {
    case TYPE_INT:
        block_matrix = _aligned_malloc(size * size * sizeof(int), MEMALIGN);
        break;
    case TYPE_FLOAT:
        block_matrix = _aligned_malloc(size * size * sizeof(float), MEMALIGN);
        break;
    case TYPE_DOUBLE:
        block_matrix = _aligned_malloc(size * size * sizeof(double), MEMALIGN);
        break;
    default:
        // Manejar tipos no definidos
        return NULL;
    }
#else
    int result;
    switch (type)
    {
    case TYPE_INT:
        result = posix_memalign(&block_matrix, MEMALIGN, size * size * sizeof(int));
        break;
    case TYPE_FLOAT:
        result = posix_memalign(&block_matrix, MEMALIGN, size * size * sizeof(float));
        break;
    case TYPE_DOUBLE:
        result = posix_memalign(&block_matrix, MEMALIGN, size * size * sizeof(double));
        break;
    default:
        // Manejar tipos no definidos
        return NULL;
    }

    if (result != 0)
    {
        fprintf(stderr, "Error: Allocation failed with error code %d.\n", result);
        return NULL;
    }
#endif

    if (block_matrix == NULL)
    {
        fprintf(stderr, "Error: Allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED); // Allocation failed
    }
    double value;

    for (unsigned int I = 0; I < r; I++)
        for (unsigned int J = 0; J < r; J++)
            for (unsigned int i = 0; i < BLOCK_SIZE; i++)
                for (unsigned int j = 0; j < BLOCK_SIZE; j++)
                    switch (type)
                    {
                    case TYPE_INT:
                        ((int *)block_matrix)[I * size * BLOCK_SIZE + J * block_size + i * BLOCK_SIZE + j] =
                            ((int *)matrix)[I * size * BLOCK_SIZE + J * BLOCK_SIZE + i * size + j];
                        break;
                    case TYPE_FLOAT:
                        ((float *)block_matrix)[I * size * BLOCK_SIZE + J * block_size + i * BLOCK_SIZE + j] =
                            ((float *)matrix)[I * size * BLOCK_SIZE + J * BLOCK_SIZE + i * size + j];
                        break;
                    case TYPE_DOUBLE:
                        value = ((double *)matrix)[I * size * BLOCK_SIZE + J * BLOCK_SIZE + i * size + j];
                        ((double *)block_matrix)[I * size * BLOCK_SIZE + J * block_size + i * BLOCK_SIZE + j] = value;
                        break;
                    default:
                        // Este caso ya fue manejado antes
                        break;
                    }

    return block_matrix;
}

void *reorganize_to_linear(void *block_matrix, unsigned int size, DataType type)
{

    unsigned int r = size / BLOCK_SIZE; // Number of blocks along one dimension
    void *original_matrix = NULL;

    // Allocate aligned memory based on data type
#if defined(_WIN32) || defined(_WIN64)
    switch (type)
    {
    case TYPE_INT:
        original_matrix = _aligned_malloc(size * size * sizeof(int), MEMALIGN);
        break;
    case TYPE_FLOAT:
        original_matrix = _aligned_malloc(size * size * sizeof(float), MEMALIGN);
        break;
    case TYPE_DOUBLE:
        original_matrix = _aligned_malloc(size * size * sizeof(double), MEMALIGN);
        break;
    default:
        // Manejar tipos no definidos
        return NULL;
    }
#else
    int result;
    switch (type)
    {
    case TYPE_INT:
        result = posix_memalign(&original_matrix, MEMALIGN, size * size * sizeof(int));
        break;
    case TYPE_FLOAT:
        result = posix_memalign(&original_matrix, MEMALIGN, size * size * sizeof(float));
        break;
    case TYPE_DOUBLE:
        result = posix_memalign(&original_matrix, MEMALIGN, size * size * sizeof(double));

        break;
    default:
        // Manejar tipos no definidos
        return NULL;
    }

    if (result != 0)
    {
        fprintf(stderr, "Error: Allocation failed with error code %d.\n", result);
        return NULL;
    }
#endif

    if (original_matrix == NULL)
    {
        fprintf(stderr, "Error: Allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED); // Allocation failed
    }

    // Copy data from the block format back to the original linear format
    for (unsigned int I = 0; I < r; I++)
        for (unsigned int J = 0; J < r; J++)
            for (unsigned int i = 0; i < BLOCK_SIZE; i++)
                for (unsigned int j = 0; j < BLOCK_SIZE; j++)
                    switch (type)
                    {
                    case TYPE_INT:
                        ((int *)original_matrix)[I * size * BLOCK_SIZE + J * BLOCK_SIZE + i * size + j] =
                            ((int *)block_matrix)[I * BLOCK_SIZE * BLOCK_SIZE * r + J * BLOCK_SIZE * BLOCK_SIZE + i * BLOCK_SIZE + j];
                        break;
                    case TYPE_FLOAT:
                        ((float *)original_matrix)[I * size * BLOCK_SIZE + J * BLOCK_SIZE + i * size + j] =
                            ((float *)block_matrix)[I * BLOCK_SIZE * BLOCK_SIZE * r + J * BLOCK_SIZE * BLOCK_SIZE + i * BLOCK_SIZE + j];
                        break;
                    case TYPE_DOUBLE:
                        ((double *)original_matrix)[I * size * BLOCK_SIZE + J * BLOCK_SIZE + i * size + j] =
                            ((double *)block_matrix)[I * BLOCK_SIZE * BLOCK_SIZE * r + J * BLOCK_SIZE * BLOCK_SIZE + i * BLOCK_SIZE + j];
                        break;
                    default:
                        // This case is already handled
                        break;
                    }

    return original_matrix;
}