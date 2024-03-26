#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "include/FW_Lib_CommonTypes.h"
#include "include/FW_Lib_Functions.h"

#define BS 128
#define TN 12

double dwalltime();
FILE *check_file(const char *);

int main(int argc, char *argv[])
{
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
        {"char", no_argument, 0, 'c'},
        {"int", no_argument, 0, 'i'},
        {"float", no_argument, 0, 'f'},
        {"double", no_argument, 0, 'd'},
        {0, 0, 0, 0}};

    int option_index = 0;

    while ((c = getopt_long(argc, argv, "p:cifd", long_options, &option_index)) != -1)
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

        default:
            fprintf(stderr, "Usage: %s -p (--path) path -c -i -f -d\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    printf("------------------------SECUENCIAL------------------------\n");
    double timetick_s = dwalltime();

    printf(" ==> Leyendo \n");
    FW_Matrix data2 = create_structure(dataType, path, BS);
    printf("%s\n", FW_details_to_string(data2));

    printf(" ==> Procesado \n");
    double timetick_s_compute = dwalltime();
    compute_FW_sequential(data2);
    double timetick_fs_compute = dwalltime();


    printf(" ==> Guardando \n");
    save_structure(data2, "./Output/", "ResultSecuential.csv", CSV, 1, 0);

    double timetick_fs = dwalltime();


    printf("------------------------PARALELO------------------------\n");
    double timetick_p = dwalltime();

    printf(" ==> Leyendo \n");
    FW_Matrix data = create_structure(dataType, path, BS);
    printf("%s\n", FW_details_to_string(data));


    printf(" ==> Procesado \n");
    double timetick_p_compute = dwalltime();
    compute_FW_paralell(data, TN); // TODO: Adjust thread num
    double timetick_fp_compute = dwalltime();

    printf(" ==> Guardando \n");
    save_structure(data, "./Output/", "ResultParalell.csv", CSV, 1, 0);
    double timetick_fp = dwalltime();

    printf("\n------------------------ Tiempos ------------------------\n");
    
    // Print Times
    printf("Tiempo Libreria Entera Paralelo %f \n", timetick_fp - timetick_p);
    printf("Tiempo Libreria Entera Secuencial %f \n", timetick_fs - timetick_s);

    printf("Tiempo Computo Paralelo %f \n", timetick_fp_compute - timetick_p_compute);
    printf("Tiempo Computo Secuencial %f \n\n", timetick_fs_compute - timetick_s_compute);

    // Free memory
    freeFW_Matrix(&data2);
    freeFW_Matrix(&data);


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

#include <sys/time.h>

double dwalltime()
{
    double sec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}
