#ifndef FW_LIB_FUNCTIONS_H
#define FW_LIB_FUNCTIONS_H

#include <sys/time.h>

#include "FW_Lib_CommonTypes.h"

/**
 * This function creates a FW_Matrix structure based on the specified data type, file path, block size, and attributes. It automatically detects the data type if not specified. 
 *
 * @param dataType The data type of the matrix elements. UNDEFINED for autodetection.
 * @param path The path to the file containing the matrix data.
 * @param BS The block size for matrix operations.
 * @param attr Pointer to FW_attr_t structure containing additional attributes.
 * @return The created FW_Matrix structure with the matrix loaded.
 */
LIB_EXPORT FW_Matrix create_structure(DataType, char *, int, FW_attr_t *);

/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function implements the the Floyd-Warshall algorithm to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'. The calculations are parallelized to improve performance.
 *
 * @param FW The FW_Matrix representing the graph.
 * @param attr The FW_attr_t pointer representing the attributes for the computation.
 */
LIB_EXPORT void compute_FW_paralell(FW_Matrix, FW_attr_t *);

/**
 * Computes the Floyd-Warshall algorithm sequentially on the given FW_Matrix to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'.
 *
 * @param FW The FW_Matrix representing the graph.
 * @param attr The FW_attr_t pointer representing the attributes for the computation.
 */
LIB_EXPORT void compute_FW_sequential(FW_Matrix, FW_attr_t *);

/**
 * @brief Saves the distance matrix or the shortest path matrix to a file.
 *
 * This function exports the distance matrix or the path matrix (as specified by the parameters) to a file
 * at the specified path. The function adjusts the file name to prevent overwrites and ensures the file format
 * is correct according to the specified type.
 *
 * @param FW The FW_Matrix structure to be saved.
 * @param path The path where the file will be saved.
 * @param name The name of the file.
 * @param fileType The type of the file (e.g., text file, binary file).
 * @param attr Additional attributes for the file (optional).
 * @note If both dist_matrix and path_matrix are set to 0, the function will not perform any action.
 */
LIB_EXPORT void save_structure(FW_Matrix, char*, char*, FileType, FW_attr_t *);


/**
 * @brief Frees the memory allocated for the FW_Matrix structure.
 * 
 * This function frees the dynamically allocated memory for the dist and path members of the FW_Matrix structure.
 * After freeing the memory, the function sets the pointers to NULL to avoid dangling pointers.
 * 
 * @param matrix A pointer to the FW_Matrix structure to be freed.
 */
LIB_EXPORT void free_FW_Matrix(FW_Matrix* matrix);

/**
 * Converts the details of a FW_Matrix element and FW_attr_t attributes into a string representation.
 *
 * @param element The FW_Matrix element containing the matrix details.
 * @param attr The FW_attr_t attributes containing additional information.
 * @return A string representation of the matrix details and attributes.
 */
LIB_EXPORT char *  FW_details_to_string(FW_Matrix * element, FW_attr_t * attr);

/**
 * Prints the FW_Matrixes.
 *
 * This function prints to the standard output the FW_Matrixes based on the given parameters.
 *
 * @param FW A pointer to the FW_Matrix structure.
 * @param print A string specifying the type of printing. ("all" | "dist" | "path")
 * @param blocks Flag indicating if the printing should be in blocks or not (1 means in blocks and the function will print a matrix of nxn with n power of 2, 0  means normal printing and the function will print a matrix of nxn with n = size of the original matrix)
 */
LIB_EXPORT void print_FW_matrixes(FW_Matrix * element, char *, int blocks);


// Attribute functions

/**
 * Creates a new FW_attr_t object with default attribute values.
 *
 * @return The newly created FW_attr_t object.
 */
LIB_EXPORT FW_attr_t new_FW_attr();

/**
 * Initializes a FW_attr_t object with default attribute values.
 *
 * @param attr A pointer to the FW_attr_t object to be initialized.
 */
LIB_EXPORT void init_FW_attr(FW_attr_t *);

// Time functions

/**
 * Returns the creation time of the FW_Matrix structure.
 *
 * @return The creation time in seconds.
 */
LIB_EXPORT double get_FW_creation_time();

/**
 * Returns the processing time of the Floyd-Warshall algorithm.
 *
 * @return The processing time in seconds.
 */
LIB_EXPORT double get_FW_processing_time();

/**
 * Returns the save to file time of the FW_Matrix structure.
 *
 * @return The save time in seconds.
 */
LIB_EXPORT double get_FW_save_time();

/**
 * Returns the total time taken by the Floyd Warshall Library.
 *
 * @return The total time taken by the Floyd Warshall Library.
 */
LIB_EXPORT double get_FW_total_time();


#endif //FW_LIB_FUNCTIONS_H