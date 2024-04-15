#ifndef FW_LIB_FUNCTIONS_H
#define FW_LIB_FUNCTIONS_H

#include "FW_Lib_CommonTypes.h"


/**
 * Creates a FW_Matrix structure.
 *
 * This function creates a FW_Matrix structure based on the specified data type, file path, block size, and attributes.
 *
 * @param dataType The data type of the matrix elements.
 * @param path The file path to read the matrix data from.
 * @param BS The block size for matrix partitioning.
 * @param attr Pointer to the FW_attr_t structure containing additional attributes.
 * @return The created FW_Matrix structure.
 */
FW_Matrix create_structure(DataType, char *, int, FW_attr_t *);


/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function implements the the Floyd-Warshall algorithm to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'. The calculations are parallelized to improve performance.
 *
 * @param FW The FW_Matrix representing the graph.
 * @param attr The FW_attr_t pointer representing the attributes for the computation.
 */
void compute_FW_paralell(FW_Matrix, FW_attr_t *);



/**
 * Computes the Floyd-Warshall algorithm sequentially on the given FW_Matrix to calculate the shortest distances between all pairs
 * of vertices in a graph, using the distance matrix contained in 'FW'.
 *
 * @param FW The FW_Matrix representing the graph.
 * @param attr The FW_attr_t pointer representing the attributes for the computation.
 */
void compute_FW_sequential(FW_Matrix, FW_attr_t *);


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
void save_structure(FW_Matrix, char*, char*, FileType, FW_attr_t *);

/**
 * @brief Frees the memory allocated for the FW_Matrix structure.
 * 
 * This function frees the dynamically allocated memory for the dist and path members of the FW_Matrix structure.
 * After freeing the memory, the function sets the pointers to NULL to avoid dangling pointers.
 * 
 * @param FW A pointer to the FW_Matrix structure to be freed.
 */
void freeFW_Matrix(FW_Matrix* matrix);

/**
 * Converts the FW_Matrix element (except matrixes) to a string representation.
 *
 * @param element The FW_Matrix element to convert.
 * @return A string representation of the FW_Matrix element.
 */
char * FW_details_to_string(FW_Matrix element);

/**
 * Prints the FW_Matrix element along with its properties and optionally the distance and path matrices.
 *
 * @param element The FW_Matrix element to be printed.
 * @param dist Flag indicating whether to print the distance matrix (1) or not (0).
 * @param path Flag indicating whether to print the path matrix (1) or not (0).
 */
void print_FW(FW_Matrix element, int dist, int path, int blocks);





FW_attr_t new_FW_attr();
void init_FW_attr(FW_attr_t *);




#endif //FW_LIB_FUNCTIONS_H