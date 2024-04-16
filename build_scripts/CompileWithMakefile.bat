@echo off
REM Call mingw32-make to build the library
mingw32-make

REM Compile main.c and link with the FloydWarshall library
gcc -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall

@echo on
