@echo off
REM Create the Compiled directory if it doesn't exist
if not exist Compiled mkdir Compiled

REM Compile FileReader
gcc -c FileReader/file.c -o Compiled/file.o
gcc -c FileTypes/CSV/CSV_createMatrix.c -o Compiled/CSV_createMatrix.o
gcc -c FileTypes/CSV/CSV_Functions.c -o Compiled/CSV_Functions.o
gcc -c FileTypes/CSV/CSV_saveMatrix.c -o Compiled/CSV_saveMatrix.o
gcc -c FileTypes/JSON/JSON_CreateMatrix.c -o Compiled/JSON_CreateMatrix.o
gcc -c FileTypes/JSON/JSON_Functions.c -o Compiled/JSON_Functions.o
gcc -c FileTypes/JSON/JSON_saveMatrix.c -o Compiled/JSON_saveMatrix.o

REM Compile Floyd-Warshall
gcc -c Floyd-Warshall/floyd_version_7_8.c -o Compiled/floyd_version_7_8.o
gcc -c Floyd-Warshall/FW_Lib_Functions.c -o Compiled/FW_Lib_Functions.o

REM Compile Generator
REM Assuming there are source files in Generator, which are not shown here.

REM Compile main program
gcc -O0 -g -fopenmp main.c Compiled/file.o Compiled/CSV_createMatrix.o Compiled/CSV_Functions.o Compiled/CSV_saveMatrix.o Compiled/JSON_CreateMatrix.o Compiled/JSON_Functions.o Compiled/JSON_saveMatrix.o Compiled/floyd_version_7_8.o Compiled/FW_Lib_Functions.o -o main_executable

echo Compilation complete!
