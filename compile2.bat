@echo off
REM Compile FileReader
gcc -c FileReader/file.c -o FileReader/file.o
gcc -c FileReader/FileTypes/CSV/CSV_createMatrix.c -o FileReader/FileTypes/CSV/CSV_createMatrix.o
gcc -c FileReader/FileTypes/CSV/CSV_Functions.c -o FileReader/FileTypes/CSV/CSV_Functions.o
gcc -c FileReader/FileTypes/CSV/CSV_saveMatrix.c -o FileReader/FileTypes/CSV/CSV_saveMatrix.o
gcc -c FileReader/FileTypes/JSON/JSON_CreateMatrix.c -o FileReader/FileTypes/JSON/JSON_CreateMatrix.o
gcc -c FileReader/FileTypes/JSON/JSON_Functions.c -o FileReader/FileTypes/JSON/JSON_Functions.o
gcc -c FileReader/FileTypes/JSON/JSON_saveMatrix.c -o FileReader/FileTypes/JSON/JSON_saveMatrix.o

REM Compile Floyd-Warshall
gcc -c Floyd-Warshall/common/floyd_version_7_8.c -o Floyd-Warshall/common/floyd_version_7_8.o
gcc -c Floyd-Warshall/FW_Lib_Functions.c -o Floyd-Warshall/FW_Lib_Functions.o
gcc -c Floyd-Warshall/FW_Lib.c -o Floyd-Warshall/FW_Lib.o

REM Compile Generator
REM Assuming there are source files in Generator, which are not shown in the image.

REM Compile main program
gcc main.c FileReader/file.o FileReader/FileTypes/CSV/CSV_createMatrix.o FileReader/FileTypes/CSV/CSV_Functions.o FileReader/FileTypes/CSV/CSV_saveMatrix.o FileReader/FileTypes/JSON/JSON_CreateMatrix.o FileReader/FileTypes/JSON/JSON_Functions.o FileReader/FileTypes/JSON/JSON_saveMatrix.o Floyd-Warshall/common/floyd_version_7_8.o Floyd-Warshall/FW_Lib_Functions.o Floyd-Warshall/FW_Lib.o -o main_executable

REM Clean object files
del FileReader\*.o
del FileReader\FileTypes\CSV\*.o
del FileReader\FileTypes\JSON\*.o
del Floyd-Warshall\common\*.o
del Floyd-Warshall\*.o

echo Compilation complete!
