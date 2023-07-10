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

int calculateMatrixSize(FileType ft, FILE* file){
    switch (ft) {
        case CSV:
            return CSV_calculateMatrixSize(file);
        case JSON:
            return JSON_calculateMatrixSize(file);
    }
}

void AutoDetectDataType(FileType ft, FILE* file){
    switch (ft) {
        case CSV:
            CSV_AutoDetectDataType(file);
        case JSON:
            JSON_AutoDetectDataType(file);
    }
    rewind(file);
}

void* createMatrix(FileType ft, DataType dt, FILE* file, int size){
    switch (ft) {
        case CSV:
            return CSV_createMatrix(file, size, dt);
        case JSON:
            return JSON_createMatrix(file, size);
    }
}
