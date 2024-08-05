#include "file_handler.h"

int *initialize_path_matrix(FW_Matrix *G);

static FileType fileType;

// Setter and Getter ----------------------------------------------------------
void set_fileType(FileType type)
{
    fileType = type;
}

FileType get_fileType()
{
    return fileType;
}

// Public ---------------------------------------------------------------------
FILE *getFile(const char *filename)
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
        exit(EXIT_OPEN_FILE_ERROR);
    }
    else
    {
        if (strcmp(ext, ".csv") == 0)
        {
            set_fileType(CSV);
        }
        else if (strcmp(ext, ".json") == 0)
        {
            set_fileType(JSON);
        }
    }

    return file;
}

char *fileTypeToString()
{
    switch (fileType)
    {
    case CSV:
        return "CSV";
    case JSON:
        return "JSON";
    default:
        return "Unknown";
    }
}

int calculate_matrix_size(FileType ft, FILE *file)
{
    rewind(file);
    switch (ft)
    {
    case CSV:
        return CSV_calculate_matrix_size(file);
    case JSON:
        return JSON_calculate_matrix_size(file);
    default:
        return -1;
    }
}

DataType auto_detect_dataType(FileType ft, FILE *file)
{
    DataType dt = UNDEFINED;
    rewind(file);
    switch (ft)
    {
    case CSV:
        dt = CSV_auto_detect_dataType(file);
        break;
    case JSON:
        dt = JSON_auto_detect_dataType(file);
        break;
    }
    return dt;
}

void create_matrixes_from_file(FW_Matrix *FW, FILE *file, int no_path)
{
    rewind(file);
    switch (FW->fileType)
    {
    case CSV:
        FW->dist = create_csv_matrix(*FW, file);
        FW->decimal_length = get_max_decimal_length();
        if (no_path)
        {
            FW->path = NULL;
        }
        else
        {
            FW->path = initialize_path_matrix(FW);

            FW->path = (int *)organize_to_blocks((void *)FW->path, FW->norm_size, TYPE_INT);
        }

        FW->dist = organize_to_blocks(FW->dist, FW->norm_size, FW->datatype);

        break;
    case JSON:
        FW->dist = create_json_matrix(*FW, file);
        FW->decimal_length = get_max_decimal_length();
        if (no_path)
        {
            FW->path = NULL;
        }
        else
        {
            FW->path = initialize_path_matrix(FW);
            FW->path = (int *)organize_to_blocks((void *)FW->path, FW->norm_size, TYPE_INT);
        }
        FW->dist = organize_to_blocks(FW->dist, FW->norm_size, FW->datatype);
        break;
    }
}

void save_matrix_to_file(FW_Matrix FW, char *path, FileType ft, unsigned int dist_matrix, unsigned int no_path, unsigned int disconnected_str)
{
    switch (ft)
    {
    case CSV:
        return CSV_save_matrix(FW, path, dist_matrix, no_path, disconnected_str);
    case JSON:
        return JSON_save_matrix(FW, path, dist_matrix, no_path, disconnected_str);
    default:
        printf("Error: Invalid file type.\n");
        break;
    }
}

// Private --------------------------------------------------------------------

int *initialize_path_matrix(FW_Matrix *G)
{
    int *P;

    switch (G->datatype)
    {
    case TYPE_INT:
        // Allocate memory for the path matrix
        P = (int *)malloc(G->norm_size * G->norm_size * sizeof(int));
        if (!P)
        {
            fprintf(stderr, "Error: Allocation failed.\n");
            exit(EXIT_ALOCATION_FAILED); // Allocation failed
        }

        // Initialize the path matrix
        for (uint64_t i = 0; i < G->norm_size; i++)
            for (uint64_t j = 0; j < G->norm_size; j++)
                if (((int *)G->dist)[i * G->norm_size + j] != INT_MAX)
                    P[i * G->norm_size + j] = i;
                else
                    P[i * G->norm_size + j] = -1;
        break;
    case TYPE_FLOAT:
        // Allocate memory for the path matrix
        P = (int *)malloc(G->norm_size * G->norm_size * sizeof(float));
        if (!P)
        {
            fprintf(stderr, "Error: Allocation failed.\n");
            exit(EXIT_ALOCATION_FAILED); // Allocation failed
        }

        // Initialize the path matrix
        for (uint64_t i = 0; i < G->norm_size; i++)
            for (uint64_t j = 0; j < G->norm_size; j++)
                if (((float *)G->dist)[i * G->norm_size + j] != FLT_MAX)
                    P[i * G->norm_size + j] = i;
                else
                    P[i * G->norm_size + j] = -1.0;
        break;
    case TYPE_DOUBLE:
        // Allocate memory for the path matrix
        P = (int *)malloc(G->norm_size * G->norm_size * sizeof(double));
        if (!P)
        {
            fprintf(stderr, "Error: Allocation failed.\n");
            exit(EXIT_ALOCATION_FAILED); // Allocation failed
        }

        // Initialize the path matrix
        for (uint64_t i = 0; i < G->norm_size; i++)
            for (uint64_t j = 0; j < G->norm_size; j++)
                if (((double *)G->dist)[i * G->norm_size + j] != DBL_MAX)
                    P[i * G->norm_size + j] = i;
                else
                    P[i * G->norm_size + j] = -1.0;
        break;
    default:
        P = NULL;
    break;
    }

    return P;
}
