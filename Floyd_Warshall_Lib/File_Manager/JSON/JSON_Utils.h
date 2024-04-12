#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "../Common/common_functions.h"
#include "../Block_Operations/blocks.h"

void * JSON_create_matrix(FW_Matrix, FILE *);
void JSON_save_matrix(FW_Matrix, char *, unsigned int, unsigned int, unsigned int);


int JSON_calculate_matrix_size(FILE *);
DataType JSON_auto_detect_dataType(FILE *);

#endif //JSON_UTILS_H
