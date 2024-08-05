@echo off
REM Call mingw32-make to build the library

mingw32-make OS=Windows_NT

echo -^> Makefile Done!

REM Check if the parameter is "static" or "dynamic"
if "%1"=="static" (
    echo -^> Static linking
    REM Compile main.c and link with the static FloydWarshall library for 32 bits
    gcc -O3 -fopenmp -march=native main.c -o FW.exe -Llib -lFloydWarshall_static

) else if "%1"=="dynamic" (
    echo -^> Static linking
    REM Compile main.c and link with the dynamic FloydWarshall library for 32 bits and copy the dll
    gcc -DBUILDING_DLL -O3 -fopenmp -march=native main.c -o FW.exe -Llib -lFloydWarshall_dynamic
    copy .\lib\libFloydWarshall_dynamic.dll .\libFloydWarshall_dynamic.dll
) else (
    echo -^> DEFAULT Static linking
    REM Default to static linking if no parameter is provided
    gcc -O3 -fopenmp -march=native main.c -o FW.exe -Llib -lFloydWarshall_static
)

echo =^> Compilation finished ^<=

@echo on
