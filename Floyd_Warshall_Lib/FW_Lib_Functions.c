#include "../include/FW_Lib_Functions.h"
#include "File_Manager/file_handler.h"
#include "FW_compute.h"

#define FW_DEFAULT_BLOCK_SIZE 128
#define FW_DEFAULT_THREAD_NUM 2 // TODO: Definir thread num


// Private functions
void print_matrix(void *, unsigned int, DataType);
char *dataTypeToString(DataType);
unsigned int nextPowerOf2(unsigned int);

// Lib Functions
FW_Matrix create_structure(DataType dataType, char *path, int BS)
{
    FW_Matrix FW;
    FILE *file = NULL;

    // Open the file and set file type
    file = getFile(path);
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open the file.\n");
        exit(EXIT_FAILURE);
    }
    FW.fileType = getFileType();

    // Autodetect of datatype if not specified
    if (dataType == UNDEFINED)
    {
        FW.datatype = AutoDetectDataType(FW.fileType, file);
    }
    else
    {
        FW.datatype = dataType;
    }

    // Calculation of matrix size
    FW.size = calculateMatrixSize(FW.fileType,  file);       // Calulates rows and cols
    FW.norm_size = nextPowerOf2(FW.size); // Calculates the max size of the matrix (nxn)

    // Set Block Size
    if (BS != -1)
    {
        FW.BS = BS;
    }
    else
    {
        FW.BS = FW_DEFAULT_BLOCK_SIZE;
    }

    FW.dist = createMatrix(FW, file); // TODO: Revisar tema de espacio en memoria al pasar el FW como parametro. Se duplican las matrices?

    // FW.path = createMatrix(FW, file); // TODO: implementacion de la inicializacion de la matriz de caminos

    return FW;
}

void compute_FW_paralell(FW_Matrix FW, int threads_num)
{
    // Set Thread NUm
    if (threads_num == -1)
    {
        threads_num = FW_DEFAULT_BLOCK_SIZE;
    }

    if(FW.datatype == TYPE_INT)
    {
        compute_FW_int_paralell(FW, threads_num);
    }
    // else if(FW.datatype == TYPE_FLOAT)
    // {
    //     compute_FW_float(FW);
    // }
    // else if(FW.datatype == TYPE_DOUBLE)
    // {
    //     compute_FW_double(FW);
    // }
    else
    {
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }
}

void compute_FW_sequential(FW_Matrix FW){
    if(FW.datatype == TYPE_INT)
    {
        compute_FW_int_sequential(FW);
    }
    // else if(FW.datatype == TYPE_FLOAT)
    // {
    //     compute_FW_float(FW);
    // }
    // else if(FW.datatype == TYPE_DOUBLE)
    // {
    //     compute_FW_double(FW);
    // }
    else
    {
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }

}

void save_structure(FW_Matrix FW, char *path, char *name, FileType fileType, int dist_matrix, int path_matrix)
{
    if (dist_matrix == 0 & path_matrix == 0)
    {
        printf("Select a matrix to export\n");
        return;
    }

    // Create copies of path and name for manipulation
    char pathCopy[1024];
    char nameCopy[256]; // Assuming name length won't exceed 256, adjust as needed
    strncpy(pathCopy, path, sizeof(pathCopy) - 1);
    strncpy(nameCopy, name, sizeof(nameCopy) - 1);
    pathCopy[sizeof(pathCopy) - 1] = '\0'; // Ensure null termination
    nameCopy[sizeof(nameCopy) - 1] = '\0'; // Ensure null termination

    // Remove trailing '/' or '\' from pathCopy
    int pathLength = strlen(pathCopy);
    if (pathCopy[pathLength - 1] == '/' || pathCopy[pathLength - 1] == '\\')
    {
        pathCopy[pathLength - 1] = '\0';
    }

    // Modify nameCopy to contain content only until the first '.'
    char *dotPosition = strchr(nameCopy, '.');
    if (dotPosition != NULL)
    {
        *dotPosition = '\0';
    }

    char fullPath[1280]; // Buffer for full path
    sprintf(fullPath, "%s/%s", pathCopy, nameCopy);
    // printf("Full Path: %s\n", fullPath);

    saveMatrix(FW, fullPath, fileType, dist_matrix, path_matrix);
}

char *FW_details_to_string(FW_Matrix element)
{
    char *result = malloc(1024);
    sprintf(result, "Matrix Size: %d\nNormalized Size: %d\nBlock Size: %d\nData Type: %s\n", element.size, element.norm_size, element.BS, dataTypeToString(element.datatype));

    return result;
}

void print_FW(FW_Matrix element, int dist, int path, int blocks)
{
    printf("\nMatrix Size: %d\nNormalized Size: %d\nBlock Size: %d\nData Type: %s\n\n", element.size, element.norm_size, element.BS, dataTypeToString(element.datatype));
    if (dist == 1)
    {
        if (blocks == 1 || blocks == -1)
        {
            printf("Distance Matrix Loaded in blocks\n");
            print_matrix(element.dist, element.norm_size, element.datatype);
        }
        if ((blocks == 0 || blocks == -1))
        {
            printf("Distance Matrix Loaded Normal\n"); // TODO: Check implementation of this

            print_matrix(element.dist, element.norm_size, element.datatype);
        }
    }
    if (path == 1)
    {
        printf("Path Matrix\n");
        print_matrix(element.path, element.norm_size, TYPE_INT);
    }
    printf("\n");
}

// ------------------------------------------------------------------ Private Section ------------------------------------------------------------------


// AUX Functions
void print_matrix(void *matrix, unsigned int size, DataType dataType)
{
    int i, j;
    switch (dataType)
    {
    case TYPE_INT:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%d ", ((int *)matrix)[i * size + j]);
            }
            printf("\n");
        }
        break;
    case TYPE_FLOAT:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%f ", ((float *)matrix)[i * size + j]);
            }
            printf("\n");
        }
        break;
    case TYPE_DOUBLE:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%lf ", ((double *)matrix)[i * size + j]);
            }
            printf("\n");
        }
        break;
    case TYPE_CHAR:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                printf("%c ", ((char *)matrix)[i * size + j]);
            }
            printf("\n");
        }
        break;
    default:
        printf("Unsupported data type for printing.\n");
        break;
    }
    printf("\n");
}

char *dataTypeToString(DataType dt)
{
    char *result = malloc(30); // allocate enough memory for the prefix and the datatype string
    switch (dt)
    {
    case TYPE_INT:
        result = "INT";
        break;
    case TYPE_FLOAT:
        result = "INT";

        break;
    case TYPE_DOUBLE:
        result = "DOUBLE";
        break;
    case UNDEFINED:
    default:
        result = "UNDEFINED";
    }
    return result;
}

unsigned int nextPowerOf2(unsigned int n)
{
    unsigned count = 0;

    // First n in the below condition is for the case where n is 0
    if (n && !(n & (n - 1)))
    {
        return n;
    }

    while (n != 0)
    {
        n >>= 1;
        count += 1;
    }

    return 1 << count;
}
