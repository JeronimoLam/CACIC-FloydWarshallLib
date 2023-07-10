//
// Created by Jero on 7/10/2023.
//

#ifndef TEST_CSV_UTILS_H
#define TEST_CSV_UTILS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

ssize_t getline (char **, size_t *, FILE *);
char *trim(char *);
int CSV_calculateMatrixSize(FILE *);
#endif //TEST_CSV_UTILS_H
