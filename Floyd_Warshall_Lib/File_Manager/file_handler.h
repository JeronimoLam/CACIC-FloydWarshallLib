#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <stdint.h>

#include "./CSV/CSV_Utils.h"
#include "./JSON/JSON_Utils.h"
#include "./Block_Operations/blocks.h"

    FILE* getFile(const char*);
    void set_fileType(FileType);
    FileType get_fileType();
    char *fileTypeToString();

    int calculate_matrix_size(FileType, FILE *);

    void create_matrixes_from_file(FW_Matrix *, FILE*, int);
    void save_matrix_to_file(FW_Matrix, char*, FileType, unsigned int, unsigned int, unsigned int);

    DataType auto_detect_dataType(FileType, FILE*);

#endif //FILE_HANDLER_H
