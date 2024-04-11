#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include "../Common/common_functions.h"
#include "../Block_Operations/blocks.h"

void * JSON_createMatrix(FW_Matrix, FILE *);
void JSON_saveMatrix(FW_Matrix, char *, unsigned int, unsigned int, unsigned int);


int JSON_calculateMatrixSize(FILE *);
DataType JSON_AutoDetectDataType(FILE *);

#endif //JSON_UTILS_H
