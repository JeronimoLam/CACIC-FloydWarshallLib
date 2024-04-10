#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <float.h>
#include <limits.h>

#include "../../../include/FW_Lib_CommonTypes.h"

unsigned int getMaxDecimalLength();

void setMaxDecimalLength(unsigned int length);

char *trim(char *str);



// Utility function to check if a character is considered a delimiter for CSV
int isDelimiter(char ch);


int tokenToInt(char *);
float tokenToFloat(char *);
double tokenToDouble(char *);

// Function to dynamically allocate and read the next token (element) from the file
char *readNextToken(FILE *file);


#endif // COMMON_FUNCTIONS_H