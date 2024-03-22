#include <stdio.h>
#include <getopt.h>


#include "include/FW_Lib_Functions.h"

#include "File_Manager/file_handler.h"

int main(int argc, char *argv[]) {
    int size;
    char *path;
    FILE *file;
    int c;
    int dataTypeFlag = 0;
    DataType dataType = UNDEFINED;

    static struct option long_options[] = {
            {"path", required_argument, 0, 'p'},
            //{"absolute-path", required_argument, 0, 'a'},
            //{"relative-path", required_argument, 0, 'r'},
            {"char",          no_argument, 0, 'c'},
            {"int",           no_argument, 0, 'i'},
            {"float",         no_argument, 0, 'f'},
            {"double",        no_argument, 0, 'd'},
            {0, 0, 0, 0}
    };

    int option_index = 0;

//    while ((c = getopt_long(argc, argv, "a:r:cifd", long_options, &option_index)) != -1) {
    while ((c = getopt_long(argc, argv, "p:cifd", long_options, &option_index)) != -1) {
        switch (c) {
            case 'p':
                path = optarg;
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
                        dataType = TYPE_CHAR;
                        break;
                    case 'i':
                        dataType = TYPE_INT;
                        break;
                    case 'f':
                        dataType = TYPE_FLOAT;
                        break;
                    case 'd':
                        dataType = TYPE_DOUBLE;
                        break;
                }
                break;

            default:
                fprintf(stderr, "Usage: %s -p (--path) path -c -i -f -d\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    FW_Matrix data = create_structure(dataType, path, -1);
    print_FW(data, 1, 0);
    compute_FW(data);
    save_structure(data, "./Output/", "Result.csv", CSV, 1, 0);

    // Closes the file
    fclose(file);
    // Free memory
    //free(matrix);

    return 0;
}
