#include "../include/FW_Lib_Functions.h"
#include "File_Manager/file_handler.h"
#include "FW_compute.h"

#define DEFAULT_BLOCK_SIZE 128
#define DEFAULT_THREAD_NUM 4
#define DEFAULT_OUTPUT_FORMAT 1     // Imprime INF en lugar de -1 por defecto
#define DEFAULT_PRINT_DIST_MATRIX 1 // Imprime la matriz de distancia por defecto
#define DEFAULT_NO_PATH 1           // No imprime ni procesa la matriz de caminos por defecto

// Global Times

static double FW_creation_time = 0;
static double FW_processing_time = 0;
static double FW_save_time = 0;

// Private functions
static double dwalltime();
static void print_matrix(void *, unsigned int, DataType);
static char *dataType_to_str(DataType);
static unsigned int next_power_of_2(unsigned int);

// Lib Functions

FW_Matrix create_structure(DataType dataType, char *path, int BS, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = new_FW_attr();
    }
    else
    {
        local_attr = *attr;
    }

    FW_Matrix FW;
    FILE *file = NULL;

    // Open the file and set file type
    file = getFile(path);
    if (file == NULL)
    {
        fprintf(stderr, "Error: Could not open the file.\n");
        exit(EXIT_FAILURE);
    }
    FW.fileType = get_fileType();

    // Autodetect of datatype if not specified
    if (dataType == UNDEFINED)
    {
        FW.datatype = auto_detect_dataType(FW.fileType, file);
    }
    else
    {
        FW.datatype = dataType;
    }

    // Calculation of matrix size
    FW.size = calculate_matrix_size(FW.fileType, file); // Calulates rows and cols
    FW.norm_size = next_power_of_2(FW.size);            // Calculates the max size of the matrix (nxn)

    // Set Block Size
    if (BS != -1)
    {
        FW.BS = BS;
    }
    else
    {
        FW.BS = DEFAULT_BLOCK_SIZE;
    }

    create_matrixes_from_file(&FW, file, local_attr.no_path); // TODO: Revisar tema de espacio en memoria al pasar el FW como parametro. Se duplican las matrices?

    fclose(file);
    FW_creation_time = dwalltime() - timetick_start;
    return FW;
}

void compute_FW_paralell(FW_Matrix FW, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = new_FW_attr();
    }
    else
    {
        local_attr = *attr;
    }

    switch (FW.datatype)
    {
    case TYPE_INT:
        compute_FW_int_paralell(FW, local_attr.thread_num, local_attr.no_path);
        break;
    case TYPE_FLOAT:
        compute_FW_float_paralell(FW, local_attr.thread_num, local_attr.no_path);
        break;
    case TYPE_DOUBLE:
        compute_FW_double_paralell(FW, local_attr.thread_num, local_attr.no_path);
        break;

    default:
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }

    FW_processing_time = dwalltime() - timetick_start;
}

void compute_FW_sequential(FW_Matrix FW, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = new_FW_attr();
    }
    else
    {
        local_attr = *attr;
    }

    switch (FW.datatype)
    {
    case TYPE_INT:
        compute_FW_int_sequential(FW, local_attr.no_path);
        break;
    case TYPE_FLOAT:
        compute_FW_float_sequential(FW, local_attr.no_path);
        break;
    case TYPE_DOUBLE:
        compute_FW_double_sequential(FW, local_attr.no_path);
        break;

    default:
        fprintf(stderr, "Error: Unsupported data type for Floyd-Warshall computation.\n");
        exit(EXIT_FAILURE);
    }

    FW_processing_time = dwalltime() - timetick_start;
}

void save_structure(FW_Matrix FW, char *path, char *name, FileType fileType, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;

    if (attr == NULL)
    {
        // attr es NULL, usa valores predeterminados
        local_attr = new_FW_attr();
    }
    else
    {
        // attr no es NULL, usa los valores proporcionados
        local_attr = *attr;
    }

    if (local_attr.print_distance_matrix == 0 & local_attr.no_path == 1)
    {
        printf("Select a matrix to export\n");
        return;
    }

    // Create copies of path and name for manipulation
    char path_copy[1024];
    char name_copy[256]; // Assuming name length won't exceed 256, adjust as needed
    strncpy(path_copy, path, sizeof(path_copy) - 1);
    strncpy(name_copy, name, sizeof(name_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0'; // Ensure null termination
    name_copy[sizeof(name_copy) - 1] = '\0'; // Ensure null termination

    // Remove trailing '/' or '\' from path_copy
    int pathLength = strlen(path_copy);
    if (path_copy[pathLength - 1] == '/' || path_copy[pathLength - 1] == '\\')
    {
        path_copy[pathLength - 1] = '\0';
    }

    // Modify name_copy to contain content only until the first '.'
    char *dot_position = strchr(name_copy, '.');
    if (dot_position != NULL)
    {
        *dot_position = '\0';
    }

    char full_path[1280]; // Buffer for full path
    sprintf(full_path, "%s/%s", path_copy, name_copy);

    save_matrix_to_file(FW, full_path, fileType, local_attr.print_distance_matrix, local_attr.no_path, local_attr.text_in_output);
    FW_save_time = dwalltime() - timetick_start;
}

void free_FW_Matrix(FW_Matrix *FW)
{
    if (FW != NULL)
    {
        // Free the dynamically allocated dist member, if it's not NULL.
        if (FW->dist != NULL)
        {
            free(FW->dist);
            FW->dist = NULL; // Avoid dangling pointer
        }
        // Free the dynamically allocated path member, if it's not NULL.
        if (FW->path != NULL)
        {
            free(FW->path);
            FW->path = NULL; // Avoid dangling pointer
        }
    }
}

char *FW_details_to_string(FW_Matrix *element, FW_attr_t *attr)
{
    char *result_martix = malloc(1024);
    if (result_martix == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED);
    }

    char *result_attr = malloc(1024);
    if (result_attr == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED);
    }

    result_martix[0] = '\0';
    result_attr[0] = '\0';
    if (element != NULL)
        sprintf(result_martix, "Matrix Size: %d\nNormalized Size: %d\nBlock Size: %d\nData Type: %s\nDecimal Part: %d", element->size, element->norm_size, element->BS, dataType_to_str(element->datatype), element->decimal_length);
    if (attr != NULL)
        sprintf(result_attr, "Thread Num: %d\nNo Path: %s\nPrint distance matrix: %s\nText in output: %s", attr->thread_num, attr->no_path ? "True" : "False", attr->print_distance_matrix ? "True" : "False", attr->text_in_output ? "INF" : "-1");
    if (element != NULL && attr != NULL)
    {
        sprintf(result_martix, "%s\n\n%s", result_martix, result_attr);
        return result_martix;
    }
    return strcat(result_martix, result_attr);
}

void print_FW_matrixes(FW_Matrix *element, char *print, int blocks)
{

    if (strstr(print, "all") || strstr(print, "dist"))
    {
        if (blocks)
        {
            printf("Distance Matrix Loaded in blocks\n");
            print_matrix(element->dist, element->norm_size, element->datatype);
        }
        if (!blocks)
        {
            printf("Distance Matrix Loaded Normal\n"); // TODO: Check implementation of this
            print_matrix(reorganize_to_linear(element->dist, element->norm_size, element->BS, element->datatype), element->size, element->datatype);
        }
    }
    if (strstr(print, "all") || strstr(print, "path"))
    {
        if (blocks)
        {
            printf("Path Matrix Loaded in blocks\n");
            print_matrix(element->path, element->norm_size, TYPE_INT);
            printf("\n");
        }
        if (!blocks)
        {
            printf("Path Matrix Loaded Normal\n"); // TODO: Check implementation of this
            print_matrix(reorganize_to_linear(element->path, element->norm_size, element->BS, TYPE_INT), element->size, TYPE_INT);
        }
    }
}

//----------------------------------------------- Attr -----------------------------------------

FW_attr_t new_FW_attr()
{
    FW_attr_t attr;
    attr.text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr.print_distance_matrix = DEFAULT_PRINT_DIST_MATRIX;
    attr.no_path = DEFAULT_NO_PATH;
    attr.thread_num = DEFAULT_THREAD_NUM;

    return attr;
}

void init_FW_attr(FW_attr_t *attr)
{
    // Initialize the attributes of the FW_attr_t object
    attr->text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr->print_distance_matrix = DEFAULT_PRINT_DIST_MATRIX;
    attr->no_path = DEFAULT_NO_PATH;
    attr->thread_num = DEFAULT_THREAD_NUM;
}

// Getters for global times

double get_FW_creation_time()
{
    return FW_creation_time;
}

double get_FW_processing_time()
{
    return FW_processing_time;
}

double get_FW_save_time()
{
    return FW_save_time;
}

double get_FW_total_time()
{
    return FW_creation_time + FW_processing_time + FW_save_time;
}

// ------------------------------------------------------------------ Private Section ------------------------------------------------------------------

static double dwalltime()
{
    double sec;
    struct timeval tv;

    gettimeofday(&tv, NULL);
    sec = tv.tv_sec + tv.tv_usec / 1000000.0;
    return sec;
}

static void print_matrix(void *matrix, unsigned int size, DataType dataType)
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

static char *dataType_to_str(DataType dt)
{
    char *result = malloc(30); // allocate enough memory for the prefix and the datatype string
    if (result == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        exit(EXIT_ALOCATION_FAILED);
    }
    switch (dt)
    {
    case TYPE_INT:
        result = "INT";
        break;
    case TYPE_FLOAT:
        result = "FLOAT";
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

static unsigned int next_power_of_2(unsigned int n)
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