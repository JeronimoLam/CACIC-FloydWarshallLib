#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "include/FW_Lib_CommonTypes.h"
#include "include/FW_Lib_Functions.h"

#define BS 128
#define TN 8
#define BAD_BS_ARGUMENT 100
#define BAD_TN_ARGUMENT 101

double dwalltime();
FILE *check_file(const char *);
int try_convert_to_int(const char *, int *);
void parse_arguments(int argc, char *argv[], char **path, FILE **file, DataType *dataType, int *block_size, int *thread_num);


int main(int argc, char *argv[])
{
    int size, dataType_flag = 0;
    char *path;
    FILE *file;
    DataType dataType = UNDEFINED;

    // Default values for block size and thread number
    int block_size = BS; // Use the default block size
    int thread_num = TN; // Use the default thread number

    parse_arguments(argc, argv, &path, &file, &dataType, &block_size, &thread_num);

    // Arguments Init
    FW_attr_t attr;
    fwl_attr_init(&attr);
    attr.no_path = 0;
    attr.thread_num = thread_num;

    printf("\nFWL v1.0\n\n");
    printf("Input file: %s\n\n", path);

    printf("Execution mode: \n");
    printf("%s", fwl_attr_get_info(&attr));

    printf("\nLoading Graph ...\n");
    FW_Matrix data = fwl_matrix_create(dataType, path, block_size, &attr); // Read
    printf("%s", fwl_matrix_get_info(&data));
    printf("Done\n\n");

    printf("Computing FW Algorithm ...\n");
    fwl_matrix_parallel_search(data, &attr); // Process
    printf("Done\n\n");

    printf("Saving Results ...\n");
    fwl_matrix_save(data, "./output/", "Resultparallel.csv", CSV, &attr); // Save
    printf("Done\n\n");

    fwl_matrix_free(&data); // Free memory

    double parallel_algorithm_time = fwl_get_search_time();
    double parallel_total_time = fwl_get_total_time();
    printf("Compute Time: %lf\n", parallel_algorithm_time);

    printf("Compute Speed: ");
    if (data.datatype == TYPE_INT){
        printf("%lf GIOPS\n", get_fw_performance(&data));
    }
    else{
        printf("%lf GFLPOS\n", get_fw_performance(&data));
    }

    return 0;
    
}

/*****************************************************************/

#include <string.h>

FILE *check_file(const char *filename)
{

    // Get the extension
    char *ext = strrchr(filename, '.');

    // Check the file extension
    if (ext == NULL || (strcmp(ext, ".csv") != 0 && strcmp(ext, ".json") != 0))
    {
        fprintf(stderr, "Error: Invalid file extension. Only .csv and .json files are supported.\n");
        exit(EXIT_FAILURE);
    }

    // Try to open the file
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file.\n");
        exit(EXIT_FAILURE);
    }
}

#include <errno.h>

int try_convert_to_int(const char *str, int *converted_value)
{
    char *end;
    errno = 0;

    long value = strtol(str, &end, 10);

    // Check for errors: no digits, overflow, underflow, or extra characters
    if (end == str || errno == ERANGE || *end != '\0' || value > INT_MAX || value < INT_MIN)
    {
        return 0; // Conversion failed
    }

    *converted_value = (int)value;
    return 1; // Conversion succeeded
}

void parse_arguments(int argc, char *argv[], char **path, FILE **file, DataType *dataType, int *block_size, int *thread_num)
{
    int c, temp_options = 0;
    int dataType_flag = 0;

    static struct option long_options[] = {
        {"path", required_argument, 0, 'p'},
        {"int", no_argument, 0, 'i'},
        {"float", no_argument, 0, 'f'},
        {"double", no_argument, 0, 'd'},
        {"block-size", required_argument, 0, 'b'},
        {"thread-num", required_argument, 0, 't'},
        {0, 0, 0, 0}};

    int option_index = 0;

    while ((c = getopt_long(argc, argv, "p:ifdb:t:", long_options, &option_index)) != -1)
    {
        switch (c)
        {
        case 'p':
            *path = optarg;
            *file = check_file(optarg);
            if (*file == NULL)
            {
                exit(EXIT_FAILURE);
            }
            break;
        case 'i':
        case 'f':
        case 'd':
            if (dataType_flag)
            {
                fprintf(stderr, "Error: only one data type argument can be used at a time.\n");
                exit(EXIT_FAILURE);
            }
            dataType_flag = 1;

            switch (c)
            {
            case 'i':
                *dataType = TYPE_INT;
                break;
            case 'f':
                *dataType = TYPE_FLOAT;
                break;
            case 'd':
                *dataType = TYPE_DOUBLE;
                break;
            }
            break;

        case 'b':
            if (!try_convert_to_int(optarg, &temp_options) || temp_options <= 0)
            {
                exit(BAD_BS_ARGUMENT);
            }
            *block_size = temp_options;
            temp_options = 0;
            break;
        case 't':
            if (!try_convert_to_int(optarg, &temp_options) || temp_options <= 0)
            {
                exit(BAD_TN_ARGUMENT);
            }
            *thread_num = temp_options;
            temp_options = 0;
            break;

        default:
            fprintf(stderr, "Usage: \n"
                            "-p, --path <path>              Specify the path to the input file.\n"
                            "-i, --int                      Set the data type to integer.\n"
                            "-f, --float                    Set the data type to float.\n"
                            "-d, --double                   Set the data type to double.\n"
                            "-b, --block-size <size>        Set the block size (default: %d).\n"
                            "-t, --thread-num <number>      Set the number of threads (default: %d).\n",
                    BS, TN);
            exit(EXIT_FAILURE);
        }
    }
}
