cmake_minimum_required(VERSION 3.23)
project(Test C)

set(CMAKE_C_STANDARD 99)

add_executable(Test main.c FileReader/file.c FileReader/file.h FileTypes/CSV/CSV_createMatrix.c FileTypes/CSV/CSV_Utils.h FileTypes/CSV/CSV_Functions.c FileTypes/JSON/JSON_CreateMatrix.c FileTypes/JSON/JSON_Utils.h FileTypes/JSON/JSON_Functions.c Floyd-Warshall/FW_Lib_Functions.c Floyd-Warshall/FW_Lib.h Floyd-Warshall/FW_Lib_Functions.h Floyd-Warshall/floyd_version_7_8.c Floyd-Warshall/common/opt_0-n.c Floyd-Warshall/common/malloc/aligned.c include/floyd_warshall.h FileTypes/CSV/CSV_saveMatrix.c FileTypes/JSON/JSON_saveMatrix.c)
