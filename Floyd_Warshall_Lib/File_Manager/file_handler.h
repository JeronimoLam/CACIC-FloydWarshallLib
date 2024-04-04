#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdint.h>

#include "./CSV/CSV_Utils.h"
#include "./JSON/JSON_Utils.h"
#include "./Block_Operations/blocks.h"

    FILE* getFile(const char*);
    void setFileType(FileType);
    FileType getFileType();
    char *fileTypeToString();

    int calculateMatrixSize(FileType, FILE *);

    void createMatrixes(FW_Matrix *, FILE*, int);
    void saveMatrix(FW_Matrix, char*, FileType, int, int);

    DataType AutoDetectDataType(FileType, FILE*);

#endif //FILE_HANDLER_H
