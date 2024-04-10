#include "file_handler.h"

int *initializePathMatrix(FW_Matrix *G);
static void initBlockedPathGraph(FW_Matrix *G); // TODO: Borrar 1

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

            // printf("Path Matrix\n");
            // for (uint64_t i = 0; i < FW->norm_size; i++)
            // {
            //     for (uint64_t j = 0; j < FW->norm_size; j++)
            //         printf("%d ", ((int *)FW->path)[i * FW->norm_size + j]);
            //     printf("\n");
            // }
            // printf("\n");

            FW->path = (int *)reorganizeToBlocks((void *)FW->path, FW->norm_size, FW->BS, TYPE_INT);
        }

        FW->dist = reorganizeToBlocks(FW->dist, FW->norm_size, FW->BS, FW->datatype);

        break;
    case JSON:
        FW->dist = JSON_createMatrix(*FW, file);
        FW->path = initializePathMatrix(FW);
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

static void initBlockedPathGraph(FW_Matrix *G)
{
    G->path = (int *)malloc(G->norm_size * G->norm_size * sizeof(int));
    uint64_t I, J, i, j, blockSize, r, idx;
    r = G->norm_size / G->BS;
    blockSize = G->BS * G->BS;
    for (I = 0; I < r; I++)
    {
        for (J = 0; J < r; J++)
        {
            for (i = 0; i < G->BS; i++)
            {
                for (j = 0; j < G->BS; j++)
                {
                    // I*n*BS = offset of the entire row of blocks number I
                    // J*blockSize = offset of block J inside the row of blocks I
                    // i*BS = offset of row i inside the block J of the row of blocks I
                    // j = position inside the row i of the block J of the row of blocks I
                    idx = I * G->norm_size * G->BS + J * blockSize + i * G->BS + j;
                    if (*((int *)G->dist + idx) != INT_MAX)
                        *(G->path + idx) = J * G->BS + j;
                    else
                        *(G->path + idx) = -1;
                }
            }
        }
    }
}
