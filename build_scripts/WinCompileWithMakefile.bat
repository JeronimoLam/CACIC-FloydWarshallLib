@echo off
REM Call mingw32-make to build the library
mingw32-make

REM Compile the library as a shared library
gcc -shared -o lib\libFloydWarshall.dll lib\libFloydWarshall.a  

echo Dymnamic Library compiled!

REM Compile main.c and link with the FloydWarshall library
gcc -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall


@echo on
