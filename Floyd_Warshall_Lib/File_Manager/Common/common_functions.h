#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <string.h>
#include <ctype.h>

#include "../../../include/FW_Lib_CommonTypes.h"

void print_int_matrix_to_file(FW_Matrix *FW, FILE *file, int *matrix, unsigned int row, unsigned int col, unsigned int disconnected_str, char * string);
void print_float_matrix_to_file(FW_Matrix * FW, FILE *file, float *matrix, unsigned int row, unsigned int col, unsigned int disconnected_str, char * string);
void print_double_matrix_to_file(FW_Matrix * FW, FILE * file, double *matrix, unsigned int row, unsigned int col, unsigned int disconnected_str, char * string);


/**
 * Get the maximum decimal length.
 *
 * @return The maximum decimal length.
 */
unsigned int getMaxDecimalLength();


/**
 * Trims leading and trailing spaces from a string.
 *
 * @param str The string to be trimmed.
 * @return The trimmed string.
 */
char *trim(char *str);

/**
 * Converts a token to an integer.
 * If the token contains the string "INF" or is equal to -1, it returns INT_MAX.
 * Otherwise, it converts the token to an integer using the atoi function.
 *
 * @param token The token to convert.
 * @return The converted integer value.
 */
int tokenToInt(char *);

/**
 * Converts a token to a float.
 * If the token contains the string "INF" or is equal to -1.0, it returns FLT_MAX.
 * Otherwise, it converts the token to a float using the atof function.
 *
 * @param token The token to convert.
 * @return The converted float value.
 */
float tokenToFloat(char *);

/**
 * Converts a token to a double.
 * If the token contains the string "INF" or is equal to -1.0, it returns DBL_MAX.
 * Otherwise, it converts the token to a double using the atof function.
 *
 * @param token The token to convert.
 * @return The converted double value.
 */
double tokenToDouble(char *);

/**
 * Reads the next token from the given file.
 *
 * @param file The file to read from.
 * @return A pointer to the next token read from the file.
 *
 * @note This function reads the file character by character and does not saturate buffers.
 */
char *readNextToken(FILE *file);

/**
 * Opens a result file with the given path and extension.
 *
 * @param path The path of the result file.
 * @param extension The extension of the result file.
 * @return A pointer to the opened file, or NULL if an error occurred.
 */
FILE *open_result_file(const char *path, const char *extension);



#endif // COMMON_FUNCTIONS_H