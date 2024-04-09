#ifndef FW_LIB_COMMON_TYPES_H
#define FW_LIB_COMMON_TYPES_H

#include <limits.h>

#define INFINITE SHRT_MAX

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    UNDEFINED,
} DataType;

typedef enum {
    CSV,
    JSON,
} FileType;

typedef struct {
    void * dist;
    int * path;
    FileType fileType;
    unsigned int decimal_length;
    DataType datatype;
    unsigned int size;
    unsigned int norm_size;
    unsigned int BS;
} FW_Matrix;

typedef struct {
    unsigned int text_in_output;
    unsigned int print_distance_matrix;
    unsigned int no_path;
    unsigned int thread_num;
} FW_attr_t;

#endif //FW_LIB_COMMON_TYPES_H
