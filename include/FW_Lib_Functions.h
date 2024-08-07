#ifndef FW_LIB_FUNCTIONS_H
#define FW_LIB_FUNCTIONS_H

#include <sys/time.h>

#include "FW_Lib_CommonTypes.h"

/**
 * This function creates a FW_Matrix structure based on the specified data type, file path, and attributes. It automatically detects the data type if not specified. 
 *
 * @param dataType The data type of the matrix elements. UNDEFINED for autodetection.
 * @param path The path to the file containing the matrix data.
 * @param attr Pointer to FW_attr_t structure containing additional attributes.
 * @return The created FW_Matrix structure with the matrix loaded.
 */
LIB_EXPORT FW_Matrix fwl_matrix_create(DataType, char *, FW_attr_t *);

/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function implements the the Floyd-Warshall algorithm to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'. The calculations are parallelized to improve performance.
 *
 * @param FW The FW_Matrix representing the graph.
 * @param attr The FW_attr_t pointer representing the attributes for the computation.
 */
LIB_EXPORT void fwl_matrix_parallel_search(FW_Matrix, FW_attr_t *);

/**
 * Computes the Floyd-Warshall algorithm sequentially on the given FW_Matrix to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'.
 *
 * @param FW The FW_Matrix representing the graph.
 * @param attr The FW_attr_t pointer representing the attributes for the computation.
 */
LIB_EXPORT void fwl_matrix_sequential_search(FW_Matrix, FW_attr_t *);

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
LIB_EXPORT void fwl_matrix_save(FW_Matrix, char*, char*, FileType, FW_attr_t *);


/**
 * @brief Frees the memory allocated for the FW_Matrix structure.
 * 
 * This function frees the dynamically allocated memory for the dist and path members of the FW_Matrix structure.
 * After freeing the memory, the function sets the pointers to NULL to avoid dangling pointers.
 * 
 * @param matrix A pointer to the FW_Matrix structure to be freed.
 */
LIB_EXPORT void fwl_matrix_free(FW_Matrix* matrix);


/**
 * Retrieves information about a FW_Matrix element.
 *
 * This function takes a pointer to a FW_Matrix element and returns a string
 * containing information about the matrix.
 *
 * @param element A pointer to the FW_Matrix element.
 * @return A string containing information about the matrix.
 */
LIB_EXPORT char * fwl_matrix_get_info(FW_Matrix * element);


/**
 * Retrieves information about a FW_attr_t element.
 *
 * This function takes a pointer to a FW_attr_t element and returns a string
 * containing information about it.
 * 
 * @param attr A pointer to the FW_attr_t element.
 * @return A string containing information about the attributes.
 */
LIB_EXPORT char * fwl_attr_get_info(FW_attr_t * attr);


// Attribute functions

/**
 * Creates a new FW_attr_t object with default attribute values.
 *
 * @return The newly created FW_attr_t object.
 */
LIB_EXPORT FW_attr_t fwl_attr_new();

/**
 * Initializes a FW_attr_t object with default attribute values.
 *
 * @param attr A pointer to the FW_attr_t object to be initialized.
 */
LIB_EXPORT void fwl_attr_init(FW_attr_t *);

// Time functions

/**
 * Returns the creation time of the FW_Matrix structure.
 *
 * @return The creation time in seconds.
 */
LIB_EXPORT double fwl_get_create_time();

/**
 * Returns the processing time of the Floyd-Warshall algorithm.
 *
 * @return The processing time in seconds.
 */
LIB_EXPORT double fwl_get_search_time();

/**
 * Returns the save to file time of the FW_Matrix structure.
 *
 * @return The save time in seconds.
 */
LIB_EXPORT double fwl_get_save_time();

/**
 * Returns the total time taken by the Floyd Warshall Library.
 *
 * @return The total time taken by the Floyd Warshall Library.
 */
LIB_EXPORT double fwl_get_total_time();

/**
 * @brief Retrieves the number of floating-point operations per second (FLOPS or IOPS).
 *
 * This function returns the number of floating-point operations or integer-operations that can be performed per second by the system.
 *
 * @return The number of FLOPS or IOPS as a double.
 */
LIB_EXPORT double get_fw_performance(FW_Matrix * matrix);


#endif //FW_LIB_FUNCTIONS_H