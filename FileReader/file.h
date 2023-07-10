#ifndef TEST_FILE_H
#define TEST_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../CSVMatrixCreator/CSV_createMatrix.h"

    typedef enum {
        CSV,
        JSON,
    } FileType;

    FILE* getFile(const char*);
    void setFileType(FileType);
    FileType getFileType();
    char *fileTypeToString();

    int calculateMatrixSize(FileType, FILE*);
    void* createMatrix(FileType, DataType,  FILE*, int);

#endif //TEST_FILE_H
