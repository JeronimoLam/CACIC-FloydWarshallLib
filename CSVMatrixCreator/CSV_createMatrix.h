// File: CSVMatrixCreator/CSV_createMatrix.h

#ifndef CSV_CREATE_MATRIX_H
#define CSV_CREATE_MATRIX_H

#include <stdio.h>
#include "../DataType/dataType.h"
#include "../CSV_Utils/CSV_Utils.h"

void* CSV_createMatrix(FILE* file, int size, DataType dataType);

#endif // CSV_CREATE_MATRIX_H
