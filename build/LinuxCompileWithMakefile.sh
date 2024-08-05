#!/bin/bash
# This is a shell script to build the library and compile main.c with specified linking (static or dynamic) and architecture.

echo "Building the library..."
make
echo "-> Makefile Done!"

# Check if the parameter is "static" or "dynamic"
if [ "$1" = "static" ]; then
    echo "-> Static linking"
    # Compile main.c and link with the static FloydWarshall library
    gcc -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_static

elif [ "$1" = "dynamic" ]; then
    echo "-> Dynamic linking"
    # Compile main.c and link with the dynamic FloydWarshall library
    gcc -DBUILDING_DLL -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_dynamic -Wl,-rpath,$(pwd)/lib
    cp ./lib/libFloydWarshall_dynamic.so ./lib/FloydWarshall_dynamic.so

else
    echo "-> DEFAULT Static linking"
    # Default to static linking if no parameter is provided
    gcc -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_static
fi

echo "=> Compilation finished <="
