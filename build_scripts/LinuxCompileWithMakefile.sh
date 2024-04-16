#!/bin/bash
# Call make to build the library
make

# Compile the library as a shared library
gcc -shared -o lib/libFloydWarshall.so -fPIC lib/libFloydWarshall.a

# Compile main.c and link with the FloydWarshall library
gcc -O0 -g -fopenmp main.c -o main -Llib -lFloydWarshall -lgcc_s -lstdc++

echo "Build completed."
