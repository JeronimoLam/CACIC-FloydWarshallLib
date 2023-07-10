#ifndef TEST_DATATYPE_H
#define TEST_DATATYPE_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
    UNDEFINED,
} DataType;

void setDataType(DataType type);
DataType getDataType();
char *dataTypeToString();

#endif //TEST_DATATYPE_H
