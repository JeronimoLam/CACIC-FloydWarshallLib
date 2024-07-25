#!/bin/bash
# This is a shell script to build the library and compile main.c with specified linking (static or dynamic) and architecture.

echo "Building the library..."
make
echo "Makefile Done!"
echo "Compiling main.c with $1 linking..."


LIB_PATH=$(pwd)/lib  # Adjust this if your library is in a different relative path

# Export LD_LIBRARY_PATH to include the directory of your library
export LD_LIBRARY_PATH=$LIB_PATH:$LD_LIBRARY_PATH

# Check if the parameter is "static" or "dynamic"
if [ "$1" = "static" ]; then
    # Compile main.c and link with the static FloydWarshall library
        gcc -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_static
elif [ "$1" = "dynamic" ]; then
    # Compile main.c and link with the dynamic FloydWarshall library
        gcc -DBUILDING_DLL -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_dynamic -Wl,-rpath,$(pwd)/lib
        cp ./lib/libFloydWarshall_dynamic.so ./lib/FloydWarshall_dynamic.so
else
    # Default to static linking if no parameter is provided
    echo "Invalid parameter. Defaulting to static linking with 64 bits."
    gcc -O3 -fopenmp main.c -o main.bin -Llib -lFloydWarshall_static
fi

echo "Compilation finished."
