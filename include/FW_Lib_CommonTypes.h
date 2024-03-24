#ifndef FW_LIB_COMMON_TYPES_H
#define FW_LIB_COMMON_TYPES_H

#include <limits.h>

#define INFINITE SHRT_MAX

typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHAR,
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
    DataType datatype;
    unsigned int size;
    unsigned int norm_size;
    unsigned int BS;
} FW_Matrix;

#endif //FW_LIB_COMMON_TYPES_H
