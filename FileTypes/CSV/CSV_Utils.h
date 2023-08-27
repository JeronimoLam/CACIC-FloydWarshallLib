// File: CSV/CSV_createMatrix.h

#ifndef CSV_CREATE_MATRIX_H
#define CSV_CREATE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../Floyd-Warshall/FW_Lib.h"

void* CSV_createMatrix(FW_Matrix, FILE*);

char *trim(char *);
void CSV_calculateMatrixSize(FW_Matrix *, FILE *);
DataType CSV_AutoDetectDataType(FILE * file);

#endif // CSV_CREATE_MATRIX_H
