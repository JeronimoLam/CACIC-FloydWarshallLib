//
#include "JSON_Utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int JSON_calculateMatrixSize(FILE *file) {
    
    // Implement Function
}

DataType JSON_AutoDetectDataType(FILE * file) {
    char ch;
    int quoteFound = 0; // Bandera para detectar una comilla doble
    char buffer[20]; // Buffer para almacenar los caracteres después de la comilla doble
    int bufferIndex = 0; // Índice para el buffer
    const char *key = "\"Tipo\": \""; // La clave que estamos buscando
    char next10Chars[11]; // Buffer para almacenar los siguientes 10 caracteres
    int next10Index = 0; // Índice para el buffer de los siguientes 10 caracteres

    if (file == NULL) {
        printf("Archivo no válido.\n");
        return;
    }

    while ((ch = fgetc(file)) != EOF && next10Index < 10) {
        // Si se encontró una comilla doble, comenzar a llenar el buffer
        if (quoteFound) {
            buffer[bufferIndex++] = ch;

            // Verificar la coincidencia si llenamos el buffer hasta la longitud de la clave
            if (bufferIndex == strlen(key) || bufferIndex == sizeof(buffer) - 1) {
                buffer[bufferIndex] = '\0'; // Asegurar que el buffer es una cadena válida

                if (strcmp(buffer, key) == 0) {
                    // Si encontramos la clave, reseteamos la bandera y el índice para comenzar a almacenar los siguientes 10 caracteres
                    quoteFound = 0;
                    bufferIndex = 0;
                    continue;
                } else {
                    // Si no es una coincidencia, reiniciar la búsqueda
                    quoteFound = 0;
                    bufferIndex = 0;
                }
            }
        }

        if (ch == '\"' && !quoteFound) {
            quoteFound = 1; // Establecer la bandera al encontrar una comilla doble
            bufferIndex = 0; // Resetear el índice del buffer
            memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer
            continue; // Pasar al siguiente carácter
        }

        // Si hemos pasado la clave, comenzamos a almacenar los siguientes 10 caracteres
        if (!quoteFound && bufferIndex == 0 && next10Index < 10) {
            next10Chars[next10Index++] = ch;
        }
    }

    next10Chars[next10Index] = '\0'; // Asegurar que la cadena esté terminada correctamente

    if (next10Index > 0) {
        printf("Los siguientes 10 caracteres después de la clave son: %s\n", next10Chars);
    } else {
        printf("La clave no fue encontrada o no había suficientes caracteres después de ella.\n");
    } // Rebobinar el archivo al inicio

    // if (strcmp(type, "int") == 0) {
    //     return TYPE_INT;
    // } else if (strcmp(type, "float") == 0) {
    //     return TYPE_FLOAT;
    // } else if (strcmp(type, "double") == 0) {
    //     return TYPE_DOUBLE;
    // } else {
    //     return UNDEFINED;
    // }
    return UNDEFINED;
}
