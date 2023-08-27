#include "CSV_Utils.h"

// Trims the string
char *trim(char *str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}


// reads the whole file and calculates the size of the matrix
void CSV_calculateMatrixSize( FW_Matrix *FW, FILE *file) {
    char line[1024];
    int cols = 1, tmp_cols, rows = 0; // Start at 1 to count the first column


    while (fgets(line, sizeof(line), file)) {
        if (strlen(line) <= 1) continue; // Ignore empty lines or lines that only contain a newline character
        tmp_cols = 0;
        char* token = strtok(line, ",");
        while (token) {
            tmp_cols++;
            token = strtok(NULL, ",");
        }
        if (cols < tmp_cols) {
            cols = tmp_cols;
        }
        rows++;
    }


    rewind(file);  // Reset the file pointer to the beginning of the file
    // Set the cols and rows directly into the FW_Matrix structure using the pointer
    FW->cols = cols;
    FW->rows = rows;
}

// Scans the file looking for a '.' to determine if the data type is INT
// If '.' is found, the data type is set to FLOAT
DataType CSV_AutoDetectDataType(FILE * file) {
    char c;

    while(((c = getc(file)) != EOF)){
        if(c == '.'){
            return TYPE_FLOAT;
        }
    }
    return TYPE_INT;
}