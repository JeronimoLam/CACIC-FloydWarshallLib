#include "file.h"

static FileType fileType;

FILE* getFile(const char* filename) {
    // Get the extension
    char *ext = strrchr(filename, '.');

    // Check the file extension
    if (ext == NULL || (strcmp(ext, ".csv") != 0 && strcmp(ext, ".json") != 0)) {
        fprintf(stderr, "Error: Invalid file extension. Only .csv and .json files are supported.\n");
        exit(EXIT_FAILURE);
    }

    // Try to open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Unable to open file.\n");
        exit(EXIT_FAILURE);
    }
    else{
        if (strcmp(ext, ".csv") == 0) {
            setFileType(CSV);
        }
        else if (strcmp(ext, ".json") == 0) {
            setFileType(JSON);
        }
        printf("File Loaded\n");
    }

    return file;
}

void setFileType(FileType type) {
    fileType = type;
}

FileType getFileType() {
    return fileType;
}

char *fileTypeToString() {
    switch (fileType) {
        case CSV:
            return "CSV";
        case JSON:
            return "JSON";
        default:
            return "Unknown";
    }
}

void calculateMatrixSize(FW_Matrix *FW, FILE* file){
    switch (FW->fileType) {
        case CSV:
            CSV_calculateMatrixSize(FW, file);
            break;
        case JSON:
            JSON_calculateMatrixSize(file);
            break;
        default:
            // Handle default case
            break;
    }
}


DataType AutoDetectDataType(FileType ft, FILE* file){
    DataType dt = UNDEFINED;
    switch (ft) {
        case CSV:
            dt = CSV_AutoDetectDataType(file);
            break;
        case JSON:
            dt = JSON_AutoDetectDataType(file);
            break;
    }
    rewind(file);
    return dt;
}

void* createMatrix(FW_Matrix FW, FILE * file){
    switch (FW.fileType) {
        case CSV:
            return CSV_createMatrix(FW, file);
        case JSON:
            return JSON_createMatrix(FW, file);
    }
}

void saveMatrix(FW_Matrix * FW, char* path, int dist_matrix, int path_matrix){
    switch (FW->fileType) {
        case CSV:
            return CSV_saveMatrix(FW, path, dist_matrix, path_matrix);
        case JSON:
            return JSON_saveMatrix(FW, path, dist_matrix, path_matrix);
    }
}
