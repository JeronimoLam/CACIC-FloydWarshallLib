#include "file_handler.h"

int *initializePathMatrix(FW_Matrix *G);

static FileType fileType;

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
        exit(EXIT_FAILURE);
    }
    else
    {
        if (strcmp(ext, ".csv") == 0)
        {
            setFileType(CSV);
        }
        else if (strcmp(ext, ".json") == 0)
        {
            setFileType(JSON);
        }
    }

    return file;
}

void setFileType(FileType type)
{
    fileType = type;
}

FileType getFileType()
{
    return fileType;
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

int calculateMatrixSize(FileType ft, FILE *file)
{
    switch (ft)
    {
    case CSV:
        return CSV_calculateMatrixSize(file);
        break;
    case JSON:
        return JSON_calculateMatrixSize(file);
        break;
    default:
        // Handle default case
        break;
    }
}

DataType AutoDetectDataType(FileType ft, FILE *file)
{
    DataType dt = UNDEFINED;
    switch (ft)
    {
    case CSV:
        dt = CSV_AutoDetectDataType(file);
        break;
    case JSON:
        dt = JSON_AutoDetectDataType(file);
        break;
    }
    rewind(file);
    return dt;
}

void createMatrixes(FW_Matrix *FW, FILE *file, int no_path)
{
    switch (FW->fileType)
    {
    case CSV:
        FW->dist = CSV_createMatrix(*FW, file);
        FW->decimal_length = getMaxDecimalLength();
        if (no_path)
        {
            FW->path = NULL;
        }
        else
        {
            FW->path = initializePathMatrix(FW);

            FW->path = (int *)reorganizeToBlocks((void *)FW->path, FW->norm_size, FW->BS, TYPE_INT);
        }

        FW->dist = reorganizeToBlocks(FW->dist, FW->norm_size, FW->BS, FW->datatype);

        break;
    case JSON:
        FW->dist = JSON_createMatrix(*FW, file);
        FW->decimal_length = getMaxDecimalLength();
        if (no_path){
            FW->path = NULL;
        }
        else
        {
            FW->path = initializePathMatrix(FW);
            FW->path = (int *)reorganizeToBlocks((void *)FW->path, FW->norm_size, FW->BS, TYPE_INT);

        }
        FW->dist = reorganizeToBlocks(FW->dist, FW->norm_size, FW->BS, FW->datatype);
        break;
    }
}

void saveMatrix(FW_Matrix FW, char *path, FileType ft, unsigned int dist_matrix, unsigned int no_path, unsigned int disconnected_str)
{
    switch (fileType)
    {
    case CSV:
        return CSV_saveMatrix(FW, path, dist_matrix, no_path, disconnected_str);
    case JSON:
        return JSON_saveMatrix(FW, path, dist_matrix, no_path, disconnected_str);
    default:
        printf("Error: Invalid file type.\n");
        break;
    }
}

// Private --------------------------------------------------------------------

int *initializePathMatrix(FW_Matrix *G)
{
    int *P;

    switch (G->datatype)
    {
    case TYPE_INT:
        // Allocate memory for the path matrix
        P = (int *)malloc(G->norm_size * G->norm_size * sizeof(int));
        if (!P)
            exit(9); // Allocation failed

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
            exit(9); // Allocation failed

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
            exit(9); // Allocation failed

        // Initialize the path matrix
        for (uint64_t i = 0; i < G->norm_size; i++)
            for (uint64_t j = 0; j < G->norm_size; j++)
                if (((double *)G->dist)[i * G->norm_size + j] != DBL_MAX)
                    P[i * G->norm_size + j] = i;
                else
                    P[i * G->norm_size + j] = -1.0;
        break;
    }

    return P;
}


