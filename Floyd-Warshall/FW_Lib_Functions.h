#ifndef TEST_FW_LIB_FUNCTIONS_H
#define TEST_FW_LIB_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include "FW_Lib.h"
#include "../FileReader/file.h"

FW_Matrix create_structure(DataType, char *);
void compute_FW(FW_Matrix);
void save_structure();

#endif //TEST_FW_LIB_H