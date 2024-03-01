#include "matrix_operations.h"

#include <stdlib.h>
#include <time.h>

// Prototipos de funciones
void generarMatrizInt(int n, FILE *archivo);
void generarMatrizFloat(int n, FILE *archivo);
void guardarMatrizJSON(void *matriz, int n, char tipo, const char *path);

int main() {
    char tipo, formato;
    int n;
    char path[256];

    printf("Ingrese el tipo de numero (i = int, f = float, d = double): ");
    scanf(" %c", &tipo);

    printf("Ingrese las dimensiones de la matriz: ");
    scanf("%d", &n);

    printf("Ingrese el formato de salida (C para CVS, J para JSON): ");
    scanf(" %c", &formato);

    printf("Ingrese el path del archivo de salida: ");
    scanf("%s", path);

    srand(time(NULL)); // Inicialización del generador de números aleatorios

    switch (tipo) {
        case 'i': {
            int **matrizInt;
            generarMatrizInt(n, &matrizInt);
            if (formato == 'C') {
                guardarMatrizCSVInt(matrizInt, n, path);
            } else {
                guardarMatrizJSONInt(matrizInt, n, path);
            }
            for (int i = 0; i < n; i++) free(matrizInt[i]);
            free(matrizInt);
            break;
        }
        case 'f': {
            float **matrizFloat;
            generarMatrizFloat(n, &matrizFloat);
            if (formato == 'C') {
                guardarMatrizCSVFloat(matrizFloat, n, path);
            } else {
                guardarMatrizJSONFloat(matrizFloat, n, path);
            }
            for (int i = 0; i < n; i++) free(matrizFloat[i]);
            free(matrizFloat);
            break;
        }
        case 'd': {
            double **matrizDouble;
            generarMatrizDouble(n, &matrizDouble);
            if (formato == 'C') {
                guardarMatrizCSVDouble(matrizDouble, n, path);
            } else {
                guardarMatrizJSONDouble(matrizDouble, n, path);
            }
            for (int i = 0; i < n; i++) free(matrizDouble[i]);
            free(matrizDouble);
            break;
        }
        default:
            printf("Tipo de número no reconocido.\n");
            return 1;
    }

    printf("Archivo generado con éxito.\n");

    return 0;
}