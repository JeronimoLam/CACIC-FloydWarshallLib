# PPS
Optimizacion del algoritmo Floyd-Warshall


*Tipos de datos*
Se crearon diversos tipos y enums para contener diversa informacion en 1 mismo tipo de dato.
**Enums**
1. DataType: define el tipo de dato de la matriz [TYPE_INT, TYPE_FLOAT, TYPE_DOUBLE, UNDEFIED]
2. FileType: define el tipo de archivo de entrada [CSV, JSON]

**Structs**
1. FW_Matrix: Contiene la matriz de distancias, de caminos y los datos necesarios para el correcto funcionamiento y output del algoritmo. Los datos que contiene son:
- Matriz de Distancias (dist)
- Matriz de caminos (path)
- Tipo de archivo (fileType): Extensión del cual se leyo la matriz original. Se utiliza en el momento de crear la matriz para definir a que funciones llamar de lectura de archivos. Ejemplo, si el archivo es CSV, se deben llamar a las funciones que autodetectan longitud, tipo de dato y que crean la matriz especificas para CSV
- Longitud decimal (decimal_length): Cantidad de posiciones decimales. Se utiliza al momento de imprimir la correcta construccion.
- Tamaño de la matriz (size): tamaño de la matriz original
- Tamaño de la matriz normalizada (norm_size): tamaño de la matriz redondeado a la siguiente potencia de 2 
- Tamaño de Bloque (BS): Define el tamaño de bloque utilizado para almacenar las matrices

2. FW_attr_t: Contiene definiciones para configuraciones adicionales de las librerias. Para cargar los defaults, se debe inicializar llamando a las funciones new_FW_attr o init_FW_attr. Los valores default pueden modificarse cambiando las macros del archivo FW_Lib_Functions.c:
- text_in_output: Define si al imprimir la matriz, se utiliza el string "INF" o ekl valor -1 para indicar nodos que esten desconectados. [Default: 1 ("INF")]
- print_distance_matrix: Define si se desea imprimir la matriz de distancias al llamar a la funcion output [Default: 1 (si)]
- no_path: determina si se inicializa, procesa e imprime la matriz de caminos. [Default: 0 (no)]
- thread_num: Define la cantidad de threads para el computo paralelo del algoritmo [Default: 16]

*Input de datos*

1. La librería lee archivos CSV con numeros enteros y numeros decimales
2. Se automatizó la deteccion del tipo de dato en caso que no sea especificado por el usuario (en caso de numeros decimales, se toman automaticamente como double)
3. El archivo ahora se lee por batch para acelerar la carga de las grandes matrices.
4. Se permite que el archivo de entrada tenga el string "INF" o el numero -1 para indicar qu e un nodo NO tiene conexion con otro
5. Los datos de entrada son normalizados a la siguiente potencia de 2 para su correcto funcionamiento rellenando los vacios con valores maximos como INT_MAX, FLT_MAX y DBL_MAX
6. Con el tipo FW_attr_t se puede enviar si crear la matriz de caminos o no.

*Procesamiento de Datos*

Atencion: El correcto procesamiento de los datos esta sujeto a la representacion de los mismos en lso distintos tipos de datos.

1. Se implemento la version con semaforos dado que es la mas rapida.
2. Se crearon todas las funciones con sus debidas modificaciones para admitir las nuevas estructuras de datos
2. Se editaron todas las funciones para admitir int, float y double
3. Se crearon funciones para ejecutar el codigo de manera secuencial o paralela para cada tipo de dato
4. Se permite que el usuario ingrese la cantidad de threads. En caso de que no se asignen, se usa una cantidad Default (16)
5. Se permite que el usuario ingrese el tamaño de bloque. En caso de que no se asignen, se usa una cantidad Default (128)
6. Con el tipo FW_attr_t se puede enviar la cantidad de threads y si se procesa o no la matriz de caminos.


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

*JSON*
 Se admiten archivos JSON en la entrada. EL tamaño de la matriz y el tipo de dato utilizado debe ser especificado en 1 atributo del JSON
 La lectura de las matrices en los archivos se realizo CARACTER A CARACTER para no saturar buffers 

