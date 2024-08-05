#ifndef FW_COMPUTE_H
#define FW_COMPUTE_H

#include "../include/FW_Lib_CommonTypes.h"

#include <omp.h>
#include <semaphore.h>
#include <stdint.h>


void compute_FW_int_parallel(FW_Matrix FW, int threads_num, int no_path);
void compute_FW_int_sequential(FW_Matrix FW, int no_path);

void compute_FW_float_parallel(FW_Matrix FW, int threads_num, int no_path);
void compute_FW_float_sequential(FW_Matrix FW, int no_path);

void compute_FW_double_parallel(FW_Matrix FW, int threads_num, int no_path);
void compute_FW_double_sequential(FW_Matrix FW, int no_path);




#endif //FW_COMPUTE_H