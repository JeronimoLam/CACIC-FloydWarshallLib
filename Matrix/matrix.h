//
// Created by Jero on 7/9/2023.
//

#ifndef TEST_MATRIX_H
#define TEST_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

extern int * matrix;
extern int size;

// Function to calculate size of matrix
int calculateMatrixSize(FILE *file);

// Function to get a line from a file, POSIX style
ssize_t getline(char **lineptr, size_t *n, FILE *stream);

// Function to trim leading and trailing spaces from a string
char *trim(char *str);

// Function to print the matrix
void print_matrix();

#endif //TEST_MATRIX_H
