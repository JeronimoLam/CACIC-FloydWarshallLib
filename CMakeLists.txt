cmake_minimum_required(VERSION 3.23)
project(Test C)

set(CMAKE_C_STANDARD 99)

add_executable(Test main.c Matrix/matrix.c Matrix/matrix.h DataType/dataType.c DataType/dataType.h FileReader/file.c FileReader/file.h CSV/CSV_createMatrix.c CSV/CSV_Utils.h CSV/CSV_Functions.c JSON/JSON_CreateMatrix.c JSON/JSON_Utils.h JSON/JSON_Functions.c)
