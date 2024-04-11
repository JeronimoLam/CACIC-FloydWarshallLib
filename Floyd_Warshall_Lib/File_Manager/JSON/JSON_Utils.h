#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../../../include/FW_Lib_CommonTypes.h"
#include "../Block_Operations/blocks.h"
#include "../Common/common_functions.h"



void * JSON_createMatrix(FW_Matrix, FILE *);
void JSON_saveMatrix(FW_Matrix, char *, unsigned int, unsigned int, unsigned int);


int JSON_calculateMatrixSize(FILE *);
DataType JSON_AutoDetectDataType(FILE *);

#endif //JSON_UTILS_H
