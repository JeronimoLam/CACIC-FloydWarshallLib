#include "src/matrix_operations.h"

#include <time.h>
#include <string.h>


#define DEFAULT_TYPE 'i'
#define DEFAULT_SIZE 15
#define DEFAULT_FORMAT 'C'
#define DEFAULT_CSV_PATH "./output.csv"
#define DEFAULT_JSON_PATH "./output.json"

int main() {
    char tipo = DEFAULT_TYPE;
    int n = DEFAULT_SIZE;
    char formato = DEFAULT_FORMAT;
    char path[256];
    char input[256];
    const char *defaultPath;

    do {
        printf("Ingrese el tipo de numero (i = int, f = float, d = double) [default: %c]: ", DEFAULT_TYPE);
        fgets(input, sizeof(input), stdin);
        if (input[0] != '\n') {
            sscanf(input, " %c", &tipo);
        }
        if (tipo != 'i' && tipo != 'f' && tipo != 'd' && input[0] != '\n') {
            printf("Opcion no valida. Por favor, intente de nuevo.\n");
            tipo = 0; // Reset tipo to detect invalid input in the next iteration
        }
    } while (tipo != 'i' && tipo != 'f' && tipo != 'd');

    do {
        printf("Ingrese las dimensiones de la matriz [default: %d]: ", DEFAULT_SIZE);
        fgets(input, sizeof(input), stdin);
        if (input[0] != '\n') {
            if (sscanf(input, "%d", &n) != 1 || n <= 0) {
                printf("Entrada no válida. Por favor, intente de nuevo.\n");
                n = 0; // Reset n to detect invalid input in the next iteration
            }
        }
    } while (n <= 0);

    do {
        printf("Ingrese el formato de salida (C para CVS, J para JSON) [default: %c]: ", DEFAULT_FORMAT);
        fgets(input, sizeof(input), stdin);
        if (input[0] != '\n') {
            sscanf(input, " %c", &formato);
        }
        defaultPath = (formato == 'J' || formato == 'j') ? DEFAULT_JSON_PATH : DEFAULT_CSV_PATH;
        if (formato != 'C' && formato != 'J' && formato != 'c' && formato != 'j' && input[0] != '\n') {
            printf("Opcion no valida. Por favor, intente de nuevo.\n");
            formato = 0; // Reset formato to detect invalid input in the next iteration
        }
    } while (formato != 'C' && formato != 'J' && formato != 'c' && formato != 'j');

    printf("Ingrese el path del archivo de salida [default: %s]: ", defaultPath);
    fgets(input, sizeof(input), stdin);
    if (input[0] != '\n') {
        sscanf(input, "%s", path);
    } else {
        // Ensure default path is used if Enter is pressed
        strcpy(path, defaultPath);
    }

    srand(time(NULL)); // Inicialización del generador de números aleatorios

    switch (tipo) {
        case 'i': {
            int **matrizInt;
            generarMatrizInt(n, &matrizInt, 100);
            if (formato == 'C' || formato == 'c') {
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
            generarMatrizFloat(n, &matrizFloat, 100.0);
            if (formato == 'C' || formato == 'c') {
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
            generarMatrizDouble(n, &matrizDouble, 100.0);
            if (formato == 'C' || formato == 'c') {
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

    printf("Archivo generado con exito.\n\n");

    return 0;
}