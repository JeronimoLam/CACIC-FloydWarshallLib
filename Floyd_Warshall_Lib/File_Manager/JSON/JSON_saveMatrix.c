#include "JSON_Utils.h"

#define INF_STRING "\"INF\""

void static create_JSON(FW_Matrix *FW, DataType dt, int *matrix, const char *path, const char *extension, unsigned int disconnected_str);

void static print_JSON_Header(FILE *file, const char *type, unsigned int size);
void static print_JSON_Footer(FILE *file);
void static print_JSON_body(FW_Matrix *FW, DataType dt, FILE *file, unsigned int disconnected_str, void *ordered_matrix);

static char *dt_to_str(DataType dt);

void JSON_save_matrix(FW_Matrix FW, char *path, unsigned int print_dist, unsigned int no_path, unsigned int disconnected_str)
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

// ----------------------------- Private ----------------
void static create_JSON(FW_Matrix *FW, DataType dt, int *matrix, const char *path, const char *extension, unsigned int disconnected_str)
{
    void *ordered_matrix = reorganize_to_linear((void *)matrix, FW->norm_size, FW->BS, dt);

    FILE *file = open_result_file(path, extension);

    print_JSON_Header(file, dt_to_str(dt), FW->size);

    print_JSON_body(FW, dt, file, disconnected_str, ordered_matrix);

    print_JSON_Footer(file);

    fclose(file);
}

void static print_JSON_Header(FILE *file, const char *type, unsigned int size)
{
    fprintf(file, "{\n");
    fprintf(file, "  \"type\": \"%s\",\n", type);
    fprintf(file, "  \"size\": %d,\n", size);
    fprintf(file, "  \"matrix\": [\n");
}

void static print_JSON_Footer(FILE *file)
{
    fprintf(file, "\n  ]\n");
    fprintf(file, "}\n");
}

void static print_JSON_body(FW_Matrix *FW, DataType dt, FILE *file, unsigned int disconnected_str, void *ordered_matrix)
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

// Convierte un DataType a un string
static char *dt_to_str(DataType dt)
{
    char *result = malloc(30); // allocate enough memory for the prefix and the datatype string
    if (result == NULL)
    {
        fprintf(stderr, "Error: Allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED); // Allocation failed
    }

    switch (dt)
    {
    case TYPE_INT:
        result = "int";
        break;
    case TYPE_FLOAT:
        result = "float";

        break;
    case TYPE_DOUBLE:
        result = "double";
        break;
    default:
        result = "unknown";
        break;
    }
    return result;
}
