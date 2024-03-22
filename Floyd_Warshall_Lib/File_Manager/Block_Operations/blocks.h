#ifndef BLOCKS_H
#define BLOCKS_H

#include <stdlib.h>

#include "../../../include/FW_Lib_CommonTypes.h"


/**
 * Reorganizes a matrix into a block format.
 * 
 * This function takes a matrix of any data type (int, float, double),
 * the total size of the matrix, the block size, and the data type of the matrix.
 * The matrix is reorganized into a block format to improve data locality and
 * performance for certain operations. Memory for the new matrix is dynamically allocated
 * and must be freed by the caller.
 *
 * @param matrix Pointer to the original matrix.
 * @param size Size of one side of the square matrix.
 * @param BS Size of the side of the square blocks that the matrix is divided into.
 * @param type Data type of the matrix, specified by the DataType enum.
 * @return Pointer to the new matrix reorganized into block format, or NULL in case of an error.
 */
void * reorganizeToBlocks(void *matrix, unsigned int size, unsigned int BS, DataType type);

/**
 * Converts a block format matrix back to its original linear format.
 *
 * This function takes a matrix in block format of any data type (int, float, double),
 * the total size of the matrix, the block size, and the data type of the matrix. It then converts
 * the matrix back to its original linear row-major format. Memory for the new matrix is dynamically
 * allocated and must be freed by the caller.
 *
 * @param blockMatrix Pointer to the matrix in block format.
 * @param size Size of one side of the square matrix.
 * @param BS Size of the side of the square blocks that the matrix was divided into.
 * @param type Data type of the matrix, specified by the DataType enum.
 * @return Pointer to the new matrix in its original linear format, or NULL in case of an error.
 */
void * reorganizeToLinear(void *blockMatrix, unsigned int size, unsigned int BS, DataType type);


#endif // BLOCKS_H