#ifndef TEST_FILE_H
#define TEST_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CSV,
    JSON,
} FileType;

FILE* getFile(const char*);
void setFileType(FileType);
FileType getFileType();
char *fileTypeToString();

#endif //TEST_FILE_H
