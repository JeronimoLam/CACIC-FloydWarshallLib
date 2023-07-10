// File: CSV/CSV_createMatrix.h

#ifndef CSV_CREATE_MATRIX_H
#define CSV_CREATE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../../DataType/dataType.h"

void* CSV_createMatrix(FILE* file, int size, DataType dataType);

ssize_t getline (char **, size_t *, FILE *);
char *trim(char *);
int CSV_calculateMatrixSize(FILE *);
void CSV_AutoDetectDataType(FILE * file);

#endif // CSV_CREATE_MATRIX_H
