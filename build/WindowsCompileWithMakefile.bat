@echo off
REM Call mingw32-make to build the library

mingw32-make OS=Windows_NT

echo makefile Done!
echo Compiling main.c with %1 linking...

REM Check if the parameter is "static" or "dynamic"
if "%1"=="static" (
    REM Compile main.c and link with the static FloydWarshall library for 32 bits
        gcc -O3 -fopenmp main.c -o FW.exe -Llib -lFloydWarshall_static

) else if "%1"=="dynamic" (
    REM Compile main.c and link with the dynamic FloydWarshall library for 32 bits and copy the dll to current environment for runtime linking
        gcc -DBUILDING_DLL -O3 -fopenmp main.c -o FW.exe -Llib -lFloydWarshall_dynamic
        copy .\lib\libFloydWarshall_dynamic.dll .\libFloydWarshall_dynamic.dll
) else (
    REM Default to static linking if no parameter is provided
    echo Invalid parameter. Defaulting to static linking with 32 bits.
    gcc -O3 -fopenmp main.c -o FW.exe -Llib -lFloydWarshall_static
)

@echo on
