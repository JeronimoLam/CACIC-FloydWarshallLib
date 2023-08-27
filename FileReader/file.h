#ifndef TEST_FILE_H
#define TEST_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../FileTypes/CSV/CSV_Utils.h"
#include "../FileTypes/JSON/JSON_Utils.h"
#include "../Floyd-Warshall/FW_Lib.h"  // Only include the struct definition


    FILE* getFile(const char*);
    void setFileType(FileType);
    FileType getFileType();
    char *fileTypeToString();

    void calculateMatrixSize(FW_Matrix *, FILE *);

    void* createMatrix(FW_Matrix , FILE*);
    DataType AutoDetectDataType(FileType, FILE*);

#endif //TEST_FILE_H
