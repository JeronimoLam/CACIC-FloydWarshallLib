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

    git clone https://github.com/username/floyd-warshall-lib.git
    cd floyd-warshall-lib

### Library

#### Compile

The library can be compiled using the makefile in the root directory. Attribute OS is mandatory and it can only be "linux" or "windows". BS is optional and the defauult value is 128.

For linux:

    make OS=linux (BS=<block_size>)

For windows using mingw32:

    mingw32-make OS=windows (BS=<block_size>)

#### Library Files

makefile will create ./lib directory. All main lib files will be created in that directory so anyone can use them in any project.

### FW_app

#### FW_app Compile

In ./app directory, an example program can be found. To compile this example, use the makefile inside that dir. Attribute OS is mandatory and it can only be "linux" or "windows". Attribute LINK_TYPE define the link type between the library and the executable program. LINK_TYPE is optional and it's default value is "static".  BS is optional and the defauult value is 128.

For linux:

    make OS=linux (LINK_TYPE=[static/dynamic]) (BS=<block_size>) 

For windows using mingw32:

    mingw32-make OS=windows (LINK_TYPE=[static/dynamic]) (BS=<block_size>)

#### FW_app Usage

Once compiled, in ./app directory, an executable (FW_app.exe or FW_app.bin) will be created. The usage of this file is detailed inside FW_app.c file.

    Usage:
        "-p, --path <path>              Specify the path to the input file.
        "-i, --int                      Set the data type to integer.
        "-f, --float                    Set the data type to float.
        "-d, --double                   Set the data type to double.
        "-t, --thread-num <number>      Set the number of threads (default: get_nproc()).

### Example Generator

This subproject creates example matrixes for testing the library.

#### Generator Compile

Similar to before, the makefile must be used to compile the generator. OS attribute is mandatory and it can only be "linux" or "windows". 

For linux:

    make OS=linux

For windows using mingw32:

    mingw32-make OS=windows

#### Generator Usage

A list of inputs will be prompt to configure and specify all the characteristics of the matrix to create.
This are the following configurations

- Datatype: int/float/double
- Size
- Graph Type: Directed/ Undirected
- Integer Places (only for float or double)
- Decimal places (only for float or double)
- All decimal places in 0 (only for float or double)
- Percentage of completeness
- Output format: CSV/JSON
- Output Path

# License ??????????????


    
