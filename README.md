# Floyd-Warshall Parallel Library

## Description

This library implements the Floyd-Warshall algorithm to find the shortest paths between all pairs of nodes in a weighted graph. It offers both sequential and parallel versions of the algorithm, optimized for execution on Windows and Linux systems. The library supports reading matrices from JSON or CSV files and outputs the path and distance matrices in the same formats.

## Key Features

- **Implementations**: Sequential and parallel versions of the Floyd-Warshall algorithm.
- **Input and Output**: Support for JSON and CSV files for input and output matrices.
- **Optimization**: Utilizes blocking techniques for performance improvement, with an optimal block size of 128, based on Sergio Calderón's research.
- **Compatible Platforms**: Windows and Linux.

## Requirements

- **Compiler**: C compiler like gcc or similar.
- **Libraries**: OpenMP required for the parallel implementation.
- **Operating System**: Windows 10 or later, any Linux distribution with GCC support.

## Installation

### Clone the Repository

    ```sh
    git clone https://github.com/username/floyd-warshall-lib.git
    cd floyd-warshall-lib
    ```

### Compile the Library

#### **Linux**

   ```sh
   ./LinuxCompileWithMakefile.sh [static|dynamic]
   ```

    The script builds the library and compiles main.c with either static or dynamic linking. By default, it compiles with static linking if no parameter is provided.

#### **Windows**

```sh
WindowsCompileWithMakefile.bat [static|dynamic]
```

    The script builds the library and compiles main.c with either static or dynamic linking. By default, it compiles with static linking if no parameter is provided.

#### **Manual Compilation**

For manual compilation of the Floyd-Warshall library, the following steps are provided for both Linux and Windows platforms:

1. Linux

```sh
make
```

Static linking:

```sh
gcc -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_static
```

Dynamic linking:
```sh
    gcc -DBUILDING_DLL -march=native -O3 -fopenmp main.c -o FW.bin -Llib -lFloydWarshall_dynamic -Wl,-rpath,$(pwd)/lib
    cp ./lib/libFloydWarshall_dynamic.so ./lib/FloydWarshall_dynamic.so
```



2. Windows (using mingw32)

```sh
mingw32-make OS=Windows_NT
```

Static linking:

```sh
gcc -O3 -fopenmp -march=native main.c -o FW.exe -Llib -lFloydWarshall_static
```

Dynamic linking:

```sh
gcc -DBUILDING_DLL -O3 -fopenmp -march=native main.c -o FW.exe -Llib -lFloydWarshall_dynamic
copy .\lib\libFloydWarshall_dynamic.dll .\libFloydWarshall_dynamic.dll

```


# Usage

# Contribution


# License


    
