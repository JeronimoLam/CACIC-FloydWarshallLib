# PPS
Optimizacion del algoritmo Floyd-Warshall


*Input de datos*

1. La librería lee archivos CSV con numeros enteros y numeros decimales
2. Se automatizó la deteccion del tipo de dato en caso que no sea especificado por el usuario (en caso de numeros decimales, se toman automaticamente como double)
3. El archivo ahora se lee por batch para acelerar la carga de las grandes matrices.
4. Se permite que el archivo de entrada tenga el string "INF" o el numero -1 para indicar qu e un nodo NO tiene conexion con otro
5. Los datos de entrada son normalizados a la siguiente potencia de 2 para su correcto funcionamiento rellenando los vacios con valores maximos como INT_MAX, FLT_MAX y DBL_MAX

*Procesamiento de Datos*

Atencion: El correcto procesamiento de los datos esta sujeto a la representacion de los mismos en lso distintos tipos de datos.

1. Se implemento la version con semaforos dado que es la mas rapida.
2. Se crearon todas las funciones con sus debidas modificaciones para admitir las nuevas estructuras de datos
2. Se editaron todas las funciones para admitir int, float y double
3. Se crearon funciones para ejecutar el codigo de manera secuencial o paralela para cada tipo de dato
4. Se permite que el usuario ingrese la cantidad de threads. En caso de que no se asignen, se usa una cantidad Default (16)
5. Se permite que el usuario ingrese el tamaño de bloque. En caso de que no se asignen, se usa una cantidad Default (128)

*Output de Datos*

1. Para float y double, se automatizo el calculo de la cantidad de decimales impresos para usar la misma cantidad que el archivo de entrada
2. Se puede seleccionar si se quiere que el archivo de salida escriba -1 o "INF" para los nodos que no tienen conexion.


*Generador*

Se creo un generador de datos de prueba. Para todas las opciones se ageraron defaults para acelerar el proceso de creacion de matrices. Los mismos estan implementados con macros que pueden ser modificados al momento de compilacion del generador. Las opciones que dispone son las siguientes:

1. Tipo de numero: entreo o decimal
2. Dimensiones de la matrix
3. Tipo de Grafo (dirigido o no dirigido)
4. (Solo decimal) Cantidad de digitos para la parte entera
5. (Solo decimal) Cantidad de digitos para la parte decimal
6. (Solo decimal) Parte decimal en 0
7. Densidad del grafo: Porcentaje entre 0 y 1 para indicar la cantidad de DESCONEXIONES
8. Formato de salida: CSV o JSON
9. Path de salida

*Examples*
Se creo una carpta conejemplos para probar la libreria.

*Main*
Se creo un main que prueba la libreria. Recibe diversos argumentos de entrada:
```
"Usage: \n"
                            "-p, --path <path>              Specify the path to the input file.\n"
                            "-i, --int                      Set the data type to integer.\n"
                            "-f, --float                    Set the data type to float.\n"
                            "-d, --double                   Set the data type to double.\n"
                            "-b, --block-size <size>        Set the block size (default: %d).\n"
                            "-t, --thread-num <number>      Set the number of threads (default: %d).\n",
```

*Archivos Adicionales*

2. Se crearon archivos .bat y .sh para automatizar la compilacion de la libreria sin tener que ejecutar los comandos manualmente
3. Se crearon archivos en python para chequear tanto el creador como el resultado de la libreria


