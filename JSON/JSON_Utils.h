#ifndef TEST_JSON_UTILS_H
#define TEST_JSON_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

int JSON_calculateMatrixSize(FILE *);
int *JSON_createMatrix(FILE *,  int);

#endif //TEST_JSON_UTILS_H
