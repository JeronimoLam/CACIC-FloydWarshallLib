#ifndef TEST_FILE_H
#define TEST_FILE_H

#include "../FileTypes/CSV/CSV_Utils.h"
#include "../FileTypes/JSON/JSON_Utils.h"


    FILE* getFile(const char*);
    void setFileType(FileType);
    FileType getFileType();
    char *fileTypeToString();

    void calculateMatrixSize(FW_Matrix *, FILE *);

    void* createMatrix(FW_Matrix , FILE*);
    void saveMatrix(FW_Matrix, char*, FileType, int, int);

    DataType AutoDetectDataType(FileType, FILE*);

#endif //TEST_FILE_H
