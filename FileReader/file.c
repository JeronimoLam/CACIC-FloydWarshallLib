#include "file.h"

static FileType fileType;

FILE* getFile(const char* filename) {
    // Get the extension
    char *ext = strrchr(filename, '.');

    // Check the file extension
    if (ext == NULL || (strcmp(ext, ".csv") != 0 && strcmp(ext, ".json") != 0)) {
        printf("Error: Invalid file extension. Only .csv and .json files are supported.\n");
        return NULL;
    }

    // Try to open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Unable to open file.\n");
        return NULL;
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
