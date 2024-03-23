#ifndef TEST_FW_LIB_FUNCTIONS_H
#define TEST_FW_LIB_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "FW_Lib_CommonTypes.h"

/**
 * @brief Initializes and loads a matrix from a file, preparing it for the Floyd-Warshall algorithm.
 *
 * This function opens a file specified by 'path', detects the data type and the structure of the matrix,
 * calculates the required normalized size (power of two), and divides the matrix into blocks according to the block
 * size specified by 'BS'. Then, it loads the matrix into memory according to these parameters.
 *
 * @param dataType Data type of the matrix elements. If UNDEFINED is specified, the function will attempt to
 * auto-detect it.
 * @param path Path to the file containing the matrix.
 * @param BS Block size for matrix division. If -1 is passed, the default block size is used.
 * @return FW_Matrix Structure containing the matrix divided into blocks, among other relevant metadata.
 */
FW_Matrix create_structure(DataType, char *, int);


/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function is expected to implement the logic of the Floyd-Warshall algorithm to calculate the shortest distances
 * between all pairs of vertices in a graph, using the distance matrix contained in 'FW'. It is expected to
 * parallelize the calculations to improve performance.
 *
 * @param FW FW_Matrix structure containing the distance matrix and additional metadata necessary for
 * the calculation.
 * @note This function is pending implementation, and its description may change based on the specific
 * details of the final implementation.
 */
void compute_FW(FW_Matrix);


/**
 * @brief Saves the distance matrix or the shortest path matrix to a file.
 *
 * This function exports the distance matrix or the path matrix (as specified by the parameters) to a file
 * at the specified path. The function adjusts the file name to prevent overwrites and ensures the file format
 * is correct according to the specified type.
 *
 * @param FW The FW_Matrix structure containing the matrices to be exported.
 * @param path Path to the directory where the exported file will be saved.
 * @param name Base name for the exported file. The function may modify this name to prevent overwrites.
 * @param fileType File type for the export (e.g., binary or text).
 * @param dist_matrix Indicates whether the distance matrix should be exported (1) or not (0).
 * @param path_matrix Indicates whether the shortest path matrix should be exported (1) or not (0).
 * @note If both dist_matrix and path_matrix are set to 0, the function will not perform any action.
 */
void save_structure(FW_Matrix, char*, char*, FileType, int, int);


/**
 * Prints the details of a FW_Matrix element and optionally the distance and path matrices.
 *
 * @param element The FW_Matrix element to print.
 * @param dist    A flag indicating whether to print the distance matrix (1) or not (0).
 * @param path    A flag indicating whether to print the path matrix (1) or not (0).
 *
 * @return A string containing the details of the FW_Matrix element.
 */
char *  FW_details_to_string(FW_Matrix element, int dist, int path);

/**
 * Prints the FW_Matrix element along with its properties and optionally the distance and path matrices.
 *
 * @param element The FW_Matrix element to be printed.
 * @param dist Flag indicating whether to print the distance matrix (1) or not (0).
 * @param path Flag indicating whether to print the path matrix (1) or not (0).
 */
void print_FW(FW_Matrix element, int dist, int path);



#endif //TEST_FW_LIB_H