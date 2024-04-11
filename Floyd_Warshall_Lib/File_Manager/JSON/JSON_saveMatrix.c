#include "JSON_Utils.h"

#define INF_STRING "\"INF\"" 

void create_JSON(FW_Matrix *FW, DataType dt, int *matrix, const char *path, const char *extension, unsigned int disconnected_str);

void print_JSON_Header(FILE *file, const char *type, unsigned int size);
void print_JSON_Footer(FILE *file);
void print_JSON_body(FW_Matrix *FW, DataType dt, FILE *file, unsigned int disconnected_str, void *ordered_matrix);

void JSON_saveMatrix(FW_Matrix FW, char *path, unsigned int print_dist, unsigned int no_path, unsigned int disconnected_str)
{
    if (print_dist)
    {
        create_JSON(&FW, FW.datatype, (int *)FW.dist, path, "distances.json", disconnected_str);
    }
    if (!no_path)
    {
        create_JSON(&FW, TYPE_INT, FW.path, path, "path.json", disconnected_str);
    }
}

void create_JSON(FW_Matrix *FW, DataType dt, int *matrix, const char *path, const char *extension, unsigned int disconnected_str)
{
    void *ordered_matrix = reorganizeToLinear((void *)matrix, FW->norm_size, FW->BS, dt);

    FILE * file = open_result_file(path, extension);

    print_JSON_Header(file, "int", FW->size);

    print_JSON_body(FW, dt, file, disconnected_str, ordered_matrix);

    print_JSON_Footer(file);

    fclose(file);
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

void print_JSON_body(FW_Matrix *FW, DataType dt, FILE *file, unsigned int disconnected_str, void *ordered_matrix)
{
    for (int row = 0; row < FW->size; row++)
    {
        fprintf(file, "    [");
        for (int col = 0; col < FW->size; col++)
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
                fprintf(file, ", ");
        }
        fprintf(file, "]");
        if (row < FW->size - 1)
            fprintf(file, ",\n");
    }
}

