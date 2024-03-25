#include <stdio.h> 
#include <stdlib.h>

#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Funciones para generar matrices

/**
 * Generates a matrix of integers with random values. Its diagonal is filled with zeros.
 *
 * @param n The size of the matrix (n x n).
 * @param matriz A pointer to a pointer to store the generated matrix.
 * @param maxValue The maximum value for the elements in the matrix.
 */
void generarMatrizInt(int n, int ***matriz, int maxValue);

/**
 * Generates a matrix of floats with random values. Its diagonal is filled with zeros.
 *
 * @param n The size of the matrix (n x n).
 * @param matriz A pointer to a pointer to store the generated matrix.
 * @param maxValue The maximum value for the elements in the matrix.
 */
void generarMatrizFloat(int n, float ***matriz, float maxValue);

/**
 * Generates a matrix of dobles with random values. Its diagonal is filled with zeros.
 *
 * @param n The size of the matrix (n x n).
 * @param matriz A pointer to a pointer to store the generated matrix.
 * @param maxValue The maximum value for the elements in the matrix.
 */
void generarMatrizDouble(int n, double ***matriz, double maxValue);

// Funciones para guardar matrices en CSV
void guardarMatrizCSVInt(int **matriz, int n, const char *path);
void guardarMatrizCSVFloat(float **matriz, int n, const char *path);
void guardarMatrizCSVDouble(double **matriz, int n, const char *path);

// Funciones para guardar matrices en JSON
void guardarMatrizJSONInt(int **matriz, int n, const char *path);
void guardarMatrizJSONFloat(float **matriz, int n, const char *path);
void guardarMatrizJSONDouble(double **matriz, int n, const char *path);

#endif // MATRIX_OPERATIONS_H
