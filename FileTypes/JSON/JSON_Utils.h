#ifndef TEST_JSON_UTILS_H
#define TEST_JSON_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "../../Floyd-Warshall/FW_Lib.h"


int *JSON_createMatrix(FW_Matrix, FILE *);
void JSON_saveMatrix(FW_Matrix, char *, int, int);


void JSON_calculateMatrixSize(FILE *);
DataType JSON_AutoDetectDataType(FILE *);

#endif //TEST_JSON_UTILS_H
