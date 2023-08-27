#ifndef TEST_FW_LIB_H
#define TEST_FW_LIB_H

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
    FileType fileType;
    DataType datatype;
    int cols;
    int rows;
    int norm_size;
} FW_Matrix;

#endif //TEST_FW_LIB_H
