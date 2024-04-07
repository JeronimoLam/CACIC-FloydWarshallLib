#include <stdio.h> 
#include <stdlib.h>

#ifndef MATRIX_OPERATIONS_H
#define MATRIX_OPERATIONS_H

// Funciones para generar matrices

void IntMatrixGenerator(int n, int ***matriz, int maxValue, double porcentajeNegativos, int direction);
void DoubleMatrixGenerator(int n, double ***matriz, int parteEntera, int parteDecimal, int decimalCero, double porcentajeNegativos, int direction);

// Funciones para guardar matrices en CSV
void IntMatrix2CSV(int **matriz, int n, const char *path);
void DoubleMatrix2CSV(double **matriz, int n, const char *path, int parteDecimal, int decimalCero);

// Funciones para guardar matrices en JSON
void IntMatrix2JSON(int **matriz, int n, const char *path);
void DoubleMatrix2JSON(double **matriz, int n, const char *path, int parteDecimal, int decimalCero);

#endif // MATRIX_OPERATIONS_H
