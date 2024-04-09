#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "include/FW_Lib_CommonTypes.h"
#include "include/FW_Lib_Functions.h"

#define BS 128
#define TN 16

double dwalltime();
FILE *check_file(const char *);
int try_convert_to_int(const char *, int *);

int main(int argc, char *argv[])
{
    int size, c, dataTypeFlag = 0, temp_bs = 0, temp_tn = 0;
    char *path;
    FILE *file;
    DataType dataType = UNDEFINED;

    // Default values for block size and thread number
    int blockSize = BS; // Use the default block size
    int threadNum = TN; // Use the default thread number

    static struct option long_options[] = {
        {"path", required_argument, 0, 'p'},
        //{"absolute-path", required_argument, 0, 'a'},
        //{"relative-path", required_argument, 0, 'r'},
        // {"char", no_argument, 0, 'c'},
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
            path = optarg;
            file = check_file(optarg);
            if (file == NULL)
            {
                return -1;
            }
            break;
        case 'c':
        case 'i':
        case 'f':
        case 'd':
            if (dataTypeFlag)
            {
                fprintf(stderr, "Error: only one data type argument can be used at a time.\n");
                exit(EXIT_FAILURE);
            }
            dataTypeFlag = 1;

            switch (c)
            {
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

        case 'b':
            try_convert_to_int(optarg, &temp_bs);
            if (temp_bs == 0)
                exit(100);
            blockSize = temp_bs;
            break;
        case 't':
            try_convert_to_int(optarg, &temp_tn);
            if (temp_tn == 0)
                exit(101);
            threadNum = temp_tn;
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

    // Arguments Init

    FW_attr_t attr;
    init_FW_attr(&attr);
    // attr.text_in_output = 0;
    attr.no_path = 0;
    attr.thread_num = threadNum;
    
    printf("------------------------PARALELO------------------------\n");
    double timetick_p = dwalltime();

    // Read
    printf(" ==> Leyendo \n");
    FW_Matrix data = create_structure(dataType, path, blockSize, &attr);
    printf("%s\n", FW_details_to_string(data));
    printf("Thread Num: %d\n", threadNum);

    printf(" ==> Procesado \n");
    double timetick_p_compute = dwalltime();
    compute_FW_paralell(data, &attr); // TODO: Adjust thread num
    double timetick_fp_compute = dwalltime();

    // Save
    printf(" ==> Guardando \n");
    
    
    save_structure(data, "./Output/", "ResultParalell.csv", CSV, &attr);

    double timetick_fp = dwalltime();
    printf("Tiempo Libreria Entera Paralelo %f \n\n", timetick_fp - timetick_p);




    printf("------------------------SECUENCIAL------------------------\n");
    double timetick_s = dwalltime();

    printf(" ==> Leyendo \n");
    FW_Matrix data2 = create_structure(dataType, path, blockSize, &attr);
    printf("%s\n", FW_details_to_string(data2));
    printf("Thread Num: %d\n", threadNum);

    printf(" ==> Procesado \n");
    double timetick_s_compute = dwalltime();
    compute_FW_sequential(data2, &attr);
    double timetick_fs_compute = dwalltime();

    printf(" ==> Guardando \n");
    save_structure(data2, "./Output/", "ResultSecuential.csv", CSV, &attr);

    double timetick_fs = dwalltime();

    printf("Tiempo Computo Secuencial %f \n\n", timetick_fs_compute - timetick_s_compute);

    printf("\n------------------------ Tiempos ------------------------\n");

    // Print Times
    printf("Tiempo Libreria Entera Paralelo %f \n", timetick_fp - timetick_p);
    printf("Tiempo Libreria Entera Secuencial %f \n", timetick_fs - timetick_s);

    printf("Tiempo Computo Paralelo %f \n", timetick_fp_compute - timetick_p_compute);
    printf("Tiempo Computo Secuencial %f \n\n", timetick_fs_compute - timetick_s_compute);

    // Free memory
    // freeFW_Matrix(&data2);
    // freeFW_Matrix(&data);

    // Closes the file
    fclose(file);

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

#include <sys/time.h>

double dwalltime()
{
    double sec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}
