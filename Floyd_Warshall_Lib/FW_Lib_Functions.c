#include "../include/FW_Lib_Functions.h"
#include "File_Manager/file_handler.h"
#include "FW_compute.h"

#define DEFAULT_BLOCK_SIZE 128
#define DEFAULT_THREAD_NUM 2            // TODO: Definir thread num
#define DEFAULT_OUTPUT_FORMAT 1         // Imprime INF en lugar de -1 por defecto
#define DEFAULT_HANDLE_PATH_MATRIX 0    // No imprime ni procesa la matriz de caminos por defecto
#define DEFAULT_PRINT_DISTANCE_MATRIX 1 // Imprime la matriz de distancias por defecto

// Private functions
void print_matrix(void *, unsigned int, DataType);
char *dataTypeToString(DataType);
unsigned int nextPowerOf2(unsigned int);
int *initializePathMatrix(FW_Matrix *G);

// Lib Functions
FW_Matrix create_structure(DataType dataType, char *path, int BS, int no_path)
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
    FW.size = calculateMatrixSize(FW.fileType, file); // Calulates rows and cols
    FW.norm_size = nextPowerOf2(FW.size);             // Calculates the max size of the matrix (nxn)

    // Set Block Size
    if (BS != -1)
    {
        FW.BS = BS;
    }
    else
    {
        FW.BS = DEFAULT_BLOCK_SIZE;
    }

    createMatrixes(&FW, file, no_path); // TODO: Revisar tema de espacio en memoria al pasar el FW como parametro. Se duplican las matrices?

    // print path matrix
    // print_matrix(FW.dist, FW.norm_size, FW.datatype);
    print_matrix(FW.path, FW.norm_size, TYPE_INT);

    return FW;
}

void compute_FW_paralell(FW_Matrix FW, FW_attr_t *attr)
{
    FW_attr_t local_attr;
    if (attr == NULL)
    {
        // attr es NULL, usa valores predeterminados
        local_attr = new_FW_attr();
    }

    switch (FW.datatype)
    {
    case TYPE_INT:
        compute_FW_int_paralell(FW, attr->thread_num, attr->handle_path_matrix);
        break;
    case TYPE_FLOAT:
        compute_FW_float_paralell(FW, attr->thread_num, attr->handle_path_matrix);
        break;
    case TYPE_DOUBLE:
        compute_FW_double_paralell(FW, attr->thread_num, attr->handle_path_matrix);
        break;

    default:
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }
}

void compute_FW_sequential(FW_Matrix FW, FW_attr_t *attr)
{
    FW_attr_t local_attr;
    if (attr == NULL)
    {
        // attr es NULL, usa valores predeterminados
        local_attr = new_FW_attr();
    }

    switch (FW.datatype)
    {
    case TYPE_INT:
        compute_FW_int_sequential(FW, attr->handle_path_matrix);
        break;
    case TYPE_FLOAT:
        compute_FW_float_sequential(FW, attr->handle_path_matrix);
        break;
    case TYPE_DOUBLE:
        compute_FW_double_sequential(FW, attr->handle_path_matrix);
        break;

    default:
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }
}

void save_structure(FW_Matrix FW, char *path, char *name, FileType fileType, FW_attr_t *attr)
{
    FW_attr_t local_attr;
    if (attr == NULL)
    {
        // attr es NULL, usa valores predeterminados
        local_attr = new_FW_attr();
    }

    if (attr->print_distance_matrix == 0 & attr->handle_path_matrix == 0)
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

    saveMatrix(FW, fullPath, fileType, attr->print_distance_matrix, attr->handle_path_matrix, attr->text_in_output);
}

void freeFW_Matrix(FW_Matrix *matrix)
{
    if (matrix != NULL)
    {
        // Free the dynamically allocated dist member, if it's not NULL.
        if (matrix->dist != NULL)
        {
            free(matrix->dist);
            matrix->dist = NULL; // Avoid dangling pointer
        }
        // Free the dynamically allocated path member, if it's not NULL.
        if (matrix->path != NULL)
        {
            free(matrix->path);
            matrix->path = NULL; // Avoid dangling pointer
        }
    }
}

// Revisar cual de las 2 queda si FW_details_to_string o print_FW o  las 2
char *FW_details_to_string(FW_Matrix element)
{
    char *result = malloc(1024);
    sprintf(result, "Matrix Size: %d\nNormalized Size: %d\nBlock Size: %d\nData Type: %s\nDecimal Part: %d\n", element.size, element.norm_size, element.BS, dataTypeToString(element.datatype), element.decimal_length);

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

//----------------------------------------------- ATTR Init -----------------------------------------
FW_attr_t new_FW_attr()
{
    FW_attr_t attr;
    attr.text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr.print_distance_matrix = DEFAULT_PRINT_DISTANCE_MATRIX;
    attr.handle_path_matrix = DEFAULT_HANDLE_PATH_MATRIX;
    attr.thread_num = DEFAULT_THREAD_NUM;

    return attr;
}

void init_FW_attr(FW_attr_t *attr)
{
    attr->text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr->print_distance_matrix = DEFAULT_PRINT_DISTANCE_MATRIX;
    attr->handle_path_matrix = DEFAULT_HANDLE_PATH_MATRIX;
    attr->thread_num = DEFAULT_THREAD_NUM;
}


void print_FW_attr(FW_attr_t *attr){
    printf("Text in output: %d\n", attr->text_in_output);
    printf("Print distance matrix: %d\n", attr->print_distance_matrix);
    printf("Handle path matrix: %d\n", attr->handle_path_matrix);
    printf("Thread number: %d\n", attr->thread_num);

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
                float value = ((float *)matrix)[i * size + j];
                printf("%f ", value);
            }
            printf("\n");
        }
        break;
    case TYPE_DOUBLE:
        for (i = 0; i < size; i++)
        {
            for (j = 0; j < size; j++)
            {
                double value = ((double *)matrix)[i * size + j];
                printf("%lf ", value);
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
