// File: CSV/CSV_createMatrix.h

#ifndef CSV_CREATE_MATRIX_H
#define CSV_CREATE_MATRIX_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../../../include/FW_Lib_CommonTypes.h"
#include "../Block_Operations/blocks.h"

void * CSV_createMatrix(FW_Matrix, FILE*);
void CSV_saveMatrix(FW_Matrix,char *,  int, int);

// char *trim(char *);
void CSV_calculateMatrixSize(FW_Matrix *, FILE *);
DataType CSV_AutoDetectDataType(FILE *);

#endif // CSV_CREATE_MATRIX_H
