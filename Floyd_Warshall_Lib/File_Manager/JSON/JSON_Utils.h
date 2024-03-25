#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../../include/FW_Lib_CommonTypes.h"
#include "../Block_Operations/blocks.h"



int *JSON_createMatrix(FW_Matrix, FILE *);
void JSON_saveMatrix(FW_Matrix, char *, int, int);


int JSON_calculateMatrixSize(FILE *);
DataType JSON_AutoDetectDataType(FILE *);

#endif //JSON_UTILS_H
