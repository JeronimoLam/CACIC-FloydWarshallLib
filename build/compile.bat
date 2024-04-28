@echo off

REM Check for 'c' argument to clear the screen
if "%~1"=="c" cls

REM Create the compiled directory if it doesn't exist
if not exist compiled mkdir compiled

REM compile File Handler
gcc -c -g Floyd_Warshall_Lib/File_Manager/file_handler.c -o compiled/file_handler.o

REM For CSV
gcc -c -g Floyd_Warshall_Lib/File_Manager/CSV/CSV_createMatrix.c -o compiled/CSV_createMatrix.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/CSV/CSV_Helpers.c -o compiled/CSV_Helpers.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/CSV/CSV_saveMatrix.c -o compiled/CSV_saveMatrix.o

REM For JSON
gcc -c -g Floyd_Warshall_Lib/File_Manager/JSON/JSON_CreateMatrix.c -o compiled/JSON_CreateMatrix.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/JSON/JSON_Helpers.c -o compiled/JSON_Helpers.o
gcc -c -g Floyd_Warshall_Lib/File_Manager/JSON/JSON_saveMatrix.c -o compiled/JSON_saveMatrix.o

REM For Blocks
gcc -c -g Floyd_Warshall_Lib/File_Manager/Block_Operations/blocks.c -o compiled/blocks.o

REM For Common File Functions
gcc -c -g Floyd_Warshall_Lib/File_Manager/Common/common_functions.c -o compiled/common_functions.o

REM compile Floyd-Warshall
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_Lib_Functions.c -o compiled/FW_Lib_Functions.o -Iinclude
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_compute_int.c -o compiled/FW_compute_int.o -IFloyd_Warshall_Lib/
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_compute_float.c -o compiled/FW_compute_float.o -IFloyd_Warshall_Lib/
gcc -c -g -fopenmp Floyd_Warshall_Lib/FW_compute_double.c -o compiled/FW_compute_double.o -IFloyd_Warshall_Lib/

REM compile main program
gcc -O0 -g -fopenmp main.c compiled/file_handler.o compiled/blocks.o compiled/common_functions.o compiled/CSV_createMatrix.o compiled/CSV_Helpers.o compiled/CSV_saveMatrix.o compiled/JSON_CreateMatrix.o compiled/JSON_Helpers.o compiled/JSON_saveMatrix.o compiled/FW_Lib_Functions.o  compiled/FW_compute_int.o compiled/FW_compute_float.o compiled/FW_compute_double.o -o main_executable

echo Compilation complete!
