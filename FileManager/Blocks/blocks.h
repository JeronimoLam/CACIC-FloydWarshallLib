#ifndef BLOCKS_H
#define BLOCKS_H

//TODO check if the block size is a multiple of the matrix size
//TODO check the params
int * matrix2block_INT();
int * block2matrix_INT();

float * matrix2block_FLOAT();
float * block2matrix_FLOAT();

double * matrix2block_DOUBLE();
double * block2matrix_DOUBLE();

#endif // BLOCKS_H