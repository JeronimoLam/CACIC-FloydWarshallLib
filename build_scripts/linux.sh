#!/bin/bash

# Create the Compiled directory if it doesn't exist
if [ ! -d "Compiled" ]; then
    mkdir Compiled
fi

# Compile File Handler
gcc -c -g Floyd_Warshall_Lib/File_Manager/file_handler.c -o Compiled/file_handler.o

# For CSV
gcc -c -g Floyd_Warshall_Lib/File_Manager/CSV/CSV_createMatrix.c -o Compiled/CSV_createMatrix.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/CSV/CSV_Helpers.c -o Compiled/CSV_Helpers.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/CSV/CSV_saveMatrix.c -o Compiled/CSV_saveMatrix.o

# For JSON
gcc -c -g Floyd_Warshall_Lib/File_Manager/JSON/JSON_CreateMatrix.c -o Compiled/JSON_CreateMatrix.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/JSON/JSON_Helpers.c -o Compiled/JSON_Helpers.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/JSON/JSON_saveMatrix.c -o Compiled/JSON_saveMatrix.o

# For Common Files
gcc -c -g Floyd_Warshall_Lib/File_Manager/Common/common_functions.c -o Compiled/common_functions.o

# For Blocks
gcc -c -g Floyd_Warshall_Lib/File_Manager/Block_Operations/blocks.c -o Compiled/blocks.o

# Compile Floyd-Warshall
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_Lib_Functions.c -o Compiled/FW_Lib_Functions.o -Iinclude
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_compute_int.c -o Compiled/FW_compute_int.o -IFloyd_Warshall_Lib/
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_compute_float.c -o Compiled/FW_compute_float.o -IFloyd_Warshall_Lib/
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_compute_double.c -o Compiled/FW_compute_double.o -IFloyd_Warshall_Lib/

# Compile main program
gcc -O0 -g -fopenmp main.c Compiled/file_handler.o Compiled/blocks.o Compiled/common_functions.o Compiled/CSV_createMatrix.o Compiled/CSV_Helpers.o Compiled/CSV_saveMatrix.o Compiled/JSON_CreateMatrix.o Compiled/JSON_Helpers.o Compiled/JSON_saveMatrix.o Compiled/FW_Lib_Functions.o Compiled/FW_compute_int.o Compiled/FW_compute_float.o Compiled/FW_compute_double.o -o main_executable

echo "Compilation complete!"
