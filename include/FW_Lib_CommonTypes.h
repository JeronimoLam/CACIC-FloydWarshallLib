#ifndef FW_LIB_COMMON_TYPES_H
#define FW_LIB_COMMON_TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

#define BLOCK_SIZE 128

#if defined(_WIN32) || defined(_WIN64)
    #if defined(BUILDING_DLL)
        #define LIB_EXPORT __declspec(dllexport)
    #else
        #define LIB_EXPORT 
    #endif
#else
    #if defined(BUILDING_DLL)
        #define LIB_EXPORT __attribute__((visibility("default")))
    #else
        #define LIB_EXPORT
    #endif
#endif



#define EXIT_ALOCATION_FAILED 2
#define EXIT_OPEN_FILE_ERROR 3

typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    UNDEFINED,
} DataType;

typedef enum
{
    CSV,
    JSON,
} FileType;

typedef struct LIB_EXPORT
{
    void *dist;
    int *path;
    FileType fileType;
    unsigned int decimal_length;
    DataType datatype;
    unsigned int size;
    unsigned int norm_size;
} FW_Matrix;

typedef struct LIB_EXPORT
{
    unsigned int text_in_output;
    unsigned int print_distance_matrix;
    unsigned int no_path;
    unsigned int thread_num;
} FW_attr_t;

#endif // FW_LIB_COMMON_TYPES_H
