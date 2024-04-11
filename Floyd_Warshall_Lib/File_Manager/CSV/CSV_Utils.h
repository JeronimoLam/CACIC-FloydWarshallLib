// File: CSV/CSV_createMatrix.h

#ifndef CREATE_MATRIX_H
#define CREATE_MATRIX_H

#include "../Common/common_functions.h"
#include "../Block_Operations/blocks.h"

void * CSV_createMatrix(FW_Matrix, FILE*);
void CSV_saveMatrix(FW_Matrix, char *path, unsigned int, unsigned int, unsigned int);

// unsigned int CSV_getMaxDecimalLength();

int CSV_calculateMatrixSize(FILE *);
DataType CSV_AutoDetectDataType(FILE *);

char * readNextToken(FILE *);



#endif // CREATE_MATRIX_H
