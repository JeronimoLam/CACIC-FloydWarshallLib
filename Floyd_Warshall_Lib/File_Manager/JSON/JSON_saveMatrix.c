#include "JSON_Utils.h"

void Create_JSON(FW_Matrix *FW, DataType dt, int *matrix, const char *path, const char *extension, unsigned int disconnected_str);
void print_int_matrix_to_file(FILE *file, int *matrix, unsigned int size, unsigned int norm_size, unsigned int row, unsigned int col, unsigned int disconnected_str);
void print_float_matrix_to_file(FILE *file, float *matrix, unsigned int size, unsigned int norm_size, unsigned int row, unsigned int col, unsigned int disconnected_str);
void print_double_matrix_to_file(FILE *file, double *matrix, unsigned int size, unsigned int norm_size, unsigned int row, unsigned int col, unsigned int disconnected_str);
void print_JSON_Header(FILE *file, const char *type, unsigned int size);
void print_JSON_Footer(FILE *file);
void print_JSON_body(FW_Matrix *FW, DataType dt, FILE *file, unsigned int disconnected_str, void *ordered_matrix_int);
FILE *open_result_file(const char *path, const char *extension);

// TODO: Implement Function for all types of data
void JSON_saveMatrix(FW_Matrix FW, char *path, unsigned int print_dist, unsigned int no_path, unsigned int disconnected_str)
{
    if (print_dist)
    {
        Create_JSON(&FW, FW.datatype, (int *)FW.dist, path, "distances.json", disconnected_str);
    }
    if (!no_path)
    {
        Create_JSON(&FW, TYPE_INT, FW.path, path, "path.json", disconnected_str);
    }
}

void Create_JSON(FW_Matrix *FW, DataType dt, int *matrix, const char *path, const char *extension, unsigned int disconnected_str)
{
    void *ordered_matrix_int = reorganizeToLinear((void *)matrix, FW->norm_size, FW->BS, TYPE_INT);

    FILE *file = open_JSON_File(path, extension);

    print_JSON_Header(file, "int", FW->size);

    print_JSON_body(FW, dt, file, disconnected_str, ordered_matrix_int);

    print_JSON_Footer(file);

    fclose(file);
}

void print_int_matrix_to_file(FILE *file, int *matrix, unsigned int size, unsigned int norm_size, unsigned int row, unsigned int col, unsigned int disconnected_str)
{
    int value = ((int *)matrix)[row * norm_size + col];

    if (value == INT_MAX && disconnected_str)
    {
        fprintf(file, "\"INF\"");
    }
    else
    {
        fprintf(file, "%d", value == INT_MAX ? -1 : value);
    }
}

void print_float_matrix_to_file(FILE *file, float *matrix, unsigned int size, unsigned int norm_size, unsigned int row, unsigned int col, unsigned int disconnected_str)
{
    float value = ((int *)matrix)[row * norm_size + col];

    if (value == FLT_MAX && disconnected_str)
    {
        fprintf(file, "\"INF\"");
    }
    else
    {
        fprintf(file, "%d", value == FLT_MAX ? -1 : value);
    }
}

void print_double_matrix_to_file(FILE *file, double *matrix, unsigned int size, unsigned int norm_size, unsigned int row, unsigned int col, unsigned int disconnected_str)
{
    double value = ((double *)matrix)[row * norm_size + col];

    if (value == DBL_MAX && disconnected_str)
    {
        fprintf(file, "\"INF\"");
    }
    else
    {
        fprintf(file, "%d", value == DBL_MAX ? -1 : value);
    }
}

void print_JSON_Header(FILE *file, const char *type, unsigned int size)
{
    fprintf(file, "{\n");
    fprintf(file, "  \"type\": \"%s\",\n", type);
    fprintf(file, "  \"size\": %d,\n", size);
    fprintf(file, "  \"matrix\": [\n");
}

void print_JSON_Footer(FILE *file)
{
    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");
}

void print_JSON_body(FW_Matrix *FW, DataType dt, FILE *file, unsigned int disconnected_str, void *ordered_matrix_int)
{
    for (int row = 0; row < FW->size; row++)
    {
        fprintf(file, "    [");
        for (int col = 0; col < FW->size; col++)
        {
            switch (dt)
            {
            case TYPE_INT:
                print_int_matrix_to_file(file, (int *)ordered_matrix_int, FW->size, FW->norm_size, row, col, disconnected_str);
                break;

            case TYPE_FLOAT:
                print_float_matrix_to_file(file, (float *)ordered_matrix_int, FW->size, FW->norm_size, row, col, disconnected_str);
                break;

            case TYPE_DOUBLE:
                print_double_matrix_to_file(file, (double *)ordered_matrix_int, FW->size, FW->norm_size, row, col, disconnected_str);
                break;

            default:
                break;
            }

            if (col < FW->size - 1)
                fprintf(file, ",");
        }
        fprintf(file, "]");
        if (row < FW->size - 1)
            fprintf(file, ",\n");
    }
}

