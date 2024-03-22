gcc -O0 -g -fopenmp -o main_executable ^
main.c ^
FileReader/file.c ^
FileTypes/CSV/CSV_createMatrix.c ^
FileTypes/CSV/CSV_Functions.c ^
FileTypes/CSV/CSV_saveMatrix.c ^
FileTypes/JSON/JSON_CreateMatrix.c ^
FileTypes/JSON/JSON_Functions.c ^
FileTypes/JSON/JSON_saveMatrix.c ^
Floyd-Warshall/floyd_version_7_8.c ^
Floyd-Warshall/FW_Lib_Functions.c ^
-IFileReader ^
-IFileTypes/CSV ^
-IFileTypes/JSON ^
-IFloyd-Warshall/common