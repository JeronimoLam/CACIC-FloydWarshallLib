#ifndef FW_LIB_FUNCTIONS_H
#define FW_LIB_FUNCTIONS_H

#include <sys/time.h>

#include "FW_Lib_CommonTypes.h"

/**
 * Creates a FW_Matrix structure from a file. It automatically detects the data type if not specified. 
 *
 * @param dataType The data type of the matrix elements. UNDEFINED for autodetection.
 * @param path The path to the file containing the matrix data.
 * @param BS The block size for matrix operations.
 * @param attr Pointer to FW_attr_t structure containing additional attributes.
 * @return The created FW_Matrix structure with the matrix loaded.
 */
FW_Matrix create_structure(DataType, char *, int, FW_attr_t *);

/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function implements the logic of the Floyd-Warshall algorithm to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'. The calculations are parallelized to improve performance.
 *
 * @param FW The FW_Matrix struct containing the distance matrix and other parameters.
 * @param threads_num The number of threads to be used for parallel execution.
 * @param no_path Flag indicating whether to calculate the shortest path matrix (0) or not (1).
 */
void compute_FW_paralell(FW_Matrix, FW_attr_t *);

/**
 * Computes the Floyd-Warshall algorithm sequentially on the given FW_Matrix to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'.
 *
 * @param matrix The FW_Matrix on which to perform the computation.
 * @param no_path Flag indicating whether to calculate the shortest path matrix (0) or not (1).
 */
void compute_FW_sequential(FW_Matrix, FW_attr_t *);

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
void save_structure(FW_Matrix, char*, char*, FileType, FW_attr_t *);

/**
 * Converts the details of a FW_Matrix element and FW_attr_t attributes into a string representation.
 *
 * @param element The FW_Matrix element containing the matrix details.
 * @param attr The FW_attr_t attributes containing additional information.
 * @return A string representation of the matrix details and attributes.
 */
char *  FW_details_to_string(FW_Matrix * element, FW_attr_t * attr);

/**
 * Prints the FW_Matrix element along with its properties and optionally the distance and path matrices.
 *
 * @param element The FW_Matrix element to be printed.
 * @param dist Flag indicating whether to print the distance matrix (1) or not (0).
 * @param path Flag indicating whether to print the path matrix (1) or not (0).
 */
void print_FW_matrixes(FW_Matrix * element, char *, int blocks);

/**
 * @brief Frees the memory allocated for the FW_Matrix structure.
 * 
 * This function frees the dynamically allocated memory for the dist and path members of the FW_Matrix structure.
 * After freeing the memory, the function sets the pointers to NULL to avoid dangling pointers.
 * 
 * @param matrix A pointer to the FW_Matrix structure to be freed.
 */
void free_FW_Matrix(FW_Matrix* matrix); //TODO: ver esto

// Attribute functions

/**
 * Creates a new FW_attr_t object with default attribute values.
 *
 * @return The newly created FW_attr_t object.
 */
FW_attr_t new_FW_attr();

/**
 * Initializes a FW_attr_t object with default attribute values.
 *
 * @param attr A pointer to the FW_attr_t object to be initialized.
 */
void init_FW_attr(FW_attr_t *);

/**
 * Returns the creation time of the FW_Matrix structure.
 *
 * @return The creation time in seconds.
 */
double get_FW_creation_time();

/**
 * Returns the processing time of the Floyd-Warshall algorithm.
 *
 * @return The processing time in seconds.
 */
double get_FW_processing_time();

/**
 * Returns the save to file time of the FW_Matrix structure.
 *
 * @return The save time in seconds.
 */
double get_FW_save_time();

/**
 * Returns the total time taken by the Floyd Warshall Library.
 *
 * @return The total time taken by the Floyd Warshall Library.
 */
double get_FW_total_time();




#endif //FW_LIB_FUNCTIONS_H