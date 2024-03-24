#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include "./CSV/CSV_Utils.h"
#include "./JSON/JSON_Utils.h"


    FILE* getFile(const char*);
    void setFileType(FileType);
    FileType getFileType();
    char *fileTypeToString();

    void calculateMatrixSize(FW_Matrix *, FILE *);

    void* createMatrix(FW_Matrix , FILE*);
    void saveMatrix(FW_Matrix, char*, FileType, int, int);

    DataType AutoDetectDataType(FileType, FILE*);

#endif //FILE_HANDLER_H
