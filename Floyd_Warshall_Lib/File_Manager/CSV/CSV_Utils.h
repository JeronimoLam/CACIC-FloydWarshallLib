// File: CSV/CSV_createMatrix.h

#ifndef CREATE_MATRIX_H
#define CREATE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

#include "../../../include/FW_Lib_CommonTypes.h"
#include "../Block_Operations/blocks.h"

void * CSV_createMatrix(FW_Matrix, FILE*);
void CSV_saveMatrix(FW_Matrix,char *,  int, int);

char *trim(char *);
int CSV_calculateMatrixSize(FILE *);
DataType CSV_AutoDetectDataType(FILE *);

#endif // CREATE_MATRIX_H
