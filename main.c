#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

#include "Matrix/matrix.h"
#include "DataType/dataType.h"
#include "FileReader/file.h"

int * matrix;
int size;

FILE* getFile(const char* filename);

int main(int argc, char *argv[]) {

    FILE *file;
    int c;
    int dataTypeFlag = 0;

    static struct option long_options[] = {
            {"absolute-path", required_argument, 0, 'a'},
            {"relative-path", required_argument, 0, 'r'},
            {"char",           no_argument, 0, 'c'},
            {"int",           no_argument, 0, 'i'},
            {"float",         no_argument, 0, 'f'},
            {"double",        no_argument, 0, 'd'},
            {0, 0, 0, 0}
    };

    int option_index = 0;

    while ((c = getopt_long(argc, argv, "a:r:cifd", long_options, &option_index)) != -1) {
        switch (c) {
            case 'a':
                file = getFile(optarg);
                if (file == NULL) {
                    return -1;
                }
                break;
            case 'r':
                file = getFile(optarg);
                if (file == NULL) {
                    return -1;
                }
                break;
            case 'c':
            case 'i':
            case 'f':
            case 'd':
                if (dataTypeFlag) {
                    fprintf(stderr, "Error: only one data type argument can be used at a time.\n");
                    exit(EXIT_FAILURE);
                }
                dataTypeFlag = 1;

                switch(c) {
                    case 'c':
                        setDataType(TYPE_CHAR);
                        break;
                    case 'i':
                        setDataType(TYPE_INT);
                        break;
                    case 'f':
                        setDataType(TYPE_FLOAT);
                        break;
                    case 'd':
                        setDataType(TYPE_DOUBLE);
                        break;
                }
                break;

            default:
                fprintf(stderr, "Usage: %s --absolute-path path --relative-path path -c -i -f -d\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    printf("%s\n", dataTypeToString());
    printf("File Type: %s\n", fileTypeToString());

    // Calculation of matrix size
    size = calculateMatrixSize(file);

    matrix = (int *)malloc(size * size * sizeof(int));

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *token;
    int row = 0, col = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = '\0';

        // Split line into tokens (cells)
        token = strtok(line, ",");
        while (token != NULL) {
            token = trim(token);
            //printf("%s\t", token);
            /* if (isNumber(token))
             {
                 matrix[row * N + col] = atoi(token);
             }*/
            int test = atoi(token);
            matrix[row * size + col] = atoi(token);
            token = strtok(NULL, ",");
            col++;
        }
        col = 0;
        row++;
    }

    print_matrix();

    // Closes the file
    fclose(file);

    // Free memory
    free(matrix);
    printf("test\n");

    return 0;
}

/*
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (!fscanf(file, "%d,", &matrix[row*N + col])) {
                break;
            }
            printf("%d ", matrix[row*N + col]); // Imprime cada número a medida que se lee
        }
        printf("\n");
    }
*/




