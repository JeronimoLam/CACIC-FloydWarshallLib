#!/bin/bash
# This is a shell script to build the library and compile main.c with specified linking (static or dynamic) and architecture.

echo "Building the library..."
make
echo "Makefile Done!"
echo "Compiling main.c with $1 linking..."

# Check if the parameter is "static" or "dynamic"
if [ "$1" = "static" ]; then
    # Compile main.c and link with the static FloydWarshall library
        gcc -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_static
elif [ "$1" = "dynamic" ]; then
    # Compile main.c and link with the dynamic FloydWarshall library
        gcc -DBUILDING_DLL -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_dynamic -Wl,-rpath,$(pwd)/lib
        cp ./lib/libFloydWarshall_dynamic.so ./lib/FloydWarshall_dynamic.so
else
    # Default to static linking if no parameter is provided
    echo "Invalid parameter. Defaulting to static linking with 64 bits."
    gcc -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_static
fi

echo "Compilation finished."
