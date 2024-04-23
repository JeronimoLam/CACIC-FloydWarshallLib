#!/bin/bash
# This is a shell script to build the library and compile main.c with specified linking (static or dynamic) and architecture.

echo "Building the library..."
make
echo "Makefile Done!"
echo "Compiling main.c with $1 linking..."

# Check if the parameter is "static" or "dynamic"
if [ "$1" = "static" ]; then
    # Compile main.c and link with the static FloydWarshall library
    if [ "$2" = "32" ]; then # NOT WORKING
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
    elif [ "$2" = "64" ]; then
        gcc -m64 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
    else
        echo "Invalid architecture parameter. Defaulting to 32 bits."
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
    fi
elif [ "$1" = "dynamic" ]; then
    # Compile main.c and link with the dynamic FloydWarshall library
    if [ "$2" = "32" ]; then # NOT WORKING
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_dynamic
    elif [ "$2" = "64" ]; then
        gcc -m64 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_dynamic
    else
        echo "Invalid architecture parameter. Defaulting to 32 bits."
        gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_dynamic
    fi
else
    # Default to static linking if no parameter is provided
    echo "Invalid parameter. Defaulting to static linking with 32 bits."
    gcc -m32 -O0 -g -fopenmp main.c -o main.exe -Llib -lFloydWarshall_static
fi

echo "Compilation finished."
