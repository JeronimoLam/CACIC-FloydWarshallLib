#ifndef FW_LIB_COMMON_TYPES_H
#define FW_LIB_COMMON_TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <float.h>

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef BUILDING_LIB
        #ifdef __GNUC__
            #define LIB_EXPORT __attribute__ ((dllexport))
        #else
            #define LIB_EXPORT __declspec(dllexport) // Note: MSVC
        #endif
    #else
        #ifdef __GNUC__
            #define LIB_EXPORT __attribute__ ((dllimport))
        #else
            #define LIB_EXPORT __declspec(dllimport) // Note: MSVC
        #endif
    #endif
#else
    #if __GNUC__ >= 4
        #define LIB_EXPORT __attribute__ ((visibility ("default")))
    #else
        #define LIB_EXPORT
    #endif
#endif



#define EXIT_ALOCATION_FAILED 2

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
    unsigned int BS;
} FW_Matrix;

typedef struct LIB_EXPORT
{
    unsigned int text_in_output;
    unsigned int print_distance_matrix;
    unsigned int no_path;
    unsigned int thread_num;
} FW_attr_t;

#endif // FW_LIB_COMMON_TYPES_H
