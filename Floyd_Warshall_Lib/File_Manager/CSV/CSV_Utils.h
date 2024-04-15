// File: CSV/CSV_create_matrix.h

#ifndef CREATE_MATRIX_H
#define CREATE_MATRIX_H

#include "../Common/common_functions.h"
#include "../Block_Operations/blocks.h"

void * CSV_create_matrix(FW_Matrix, FILE*);
void CSV_save_matrix(FW_Matrix, char *path, unsigned int, unsigned int, unsigned int);


int CSV_calculate_matrix_size(FILE *);
DataType CSV_auto_detect_dataType(FILE *);

char * read_next_token(FILE *);



#endif // CREATE_MATRIX_H
