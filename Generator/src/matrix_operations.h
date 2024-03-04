#include <stdio.h> 
#include <stdlib.h>

#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Funciones para generar matrices
void generarMatrizInt(int n, int ***matriz);
void generarMatrizFloat(int n, float ***matriz);
void generarMatrizDouble(int n, double ***matriz);

// Funciones para guardar matrices en CSV
void guardarMatrizCSVInt(int **matriz, int n, const char *path);
void guardarMatrizCSVFloat(float **matriz, int n, const char *path);
void guardarMatrizCSVDouble(double **matriz, int n, const char *path);

// Funciones para guardar matrices en JSON
void guardarMatrizJSONInt(int **matriz, int n, const char *path);
void guardarMatrizJSONFloat(float **matriz, int n, const char *path);
void guardarMatrizJSONDouble(double **matriz, int n, const char *path);

#endif // MATRIX_OPERATIONS_H
