@echo off
REM Call mingw32-make to build the library

mingw32-make OS=Windows_NT

echo makefile Done!
echo Compiling main.c with %1 linking...

REM Check if the parameter is "static" or "dynamic"
if "%1"=="static" (
    REM Compile main.c and link with the static FloydWarshall library
    if "%2"=="32" (
        gcc -DBUILDING_DLL -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
    ) else if "%2"=="64" (
        REM NOT WORKING
        x86_64-w64-mingw32-gcc -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static #
    ) else (
        echo Invalid architecture parameter. Defaulting to 32 bits.
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
    )
) else if "%1"=="dynamic" (
    REM Compile main.c and link with the dynamic FloydWarshall library and copy the dll to current environment for runtime linking
    if "%2"=="32" (
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_dynamic
    ) else if "%2"=="64" (
        REM NOT WORKING
        x86_64-w64-mingw32-gcc -m64 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_dynamic
    ) else (
        echo Invalid architecture parameter. Defaulting to 32 bits.
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_dynamic
    )
) else (
    REM Default to static linking if no parameter is provided
    echo Invalid parameter. Defaulting to static linking with 32 bits.
    gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
)

@echo on
