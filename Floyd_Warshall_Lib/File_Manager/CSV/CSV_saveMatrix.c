#include <stdio.h>
#include "CSV_Utils.h"

#define INF_STRING "INF" 

void create_CSV(FW_Matrix *FW, DataType dt, void *matrix, const char *path, const char *extension, unsigned int disconnected_str);

void CSV_saveMatrix(FW_Matrix FW, char *path, unsigned int print_dist, unsigned int no_path, unsigned int disconnected_str)
{

    if (print_dist)
    {
        create_CSV(&FW, FW.datatype, (int *)FW.dist, path, "distances.csv", disconnected_str);
    }

    if (!no_path)
    {
        create_CSV(&FW, TYPE_INT,FW.path, path, "path.csv", disconnected_str);
    }
}

void create_CSV(FW_Matrix *FW, DataType dt, void *matrix, const char *path, const char *extension, unsigned int disconnected_str)
{

    void *ordered_matrix = reorganizeToLinear(matrix, FW->norm_size, FW->BS, dt);

    // Abrir el archivo
    FILE *file = open_result_file(path, extension);

    // Write the matrix to the file
    for (int row = 0; row < FW->size; ++row)
    {
        for (int col = 0; col < FW->size; ++col)
        {
            switch (dt)
            {
            case TYPE_INT:
                print_int_matrix_to_file(FW, file, (int *)ordered_matrix, row, col, disconnected_str, INF_STRING);
                break;
            case TYPE_FLOAT:
                print_float_matrix_to_file(FW, file, (float *)ordered_matrix, row, col, disconnected_str, INF_STRING);
                break;
            case TYPE_DOUBLE:
                print_double_matrix_to_file(FW, file, (double *)ordered_matrix, row, col, disconnected_str, INF_STRING);
                break;

            default:
                break;
            }
            if (col < FW->size - 1)
            {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }
    fclose(file);
}