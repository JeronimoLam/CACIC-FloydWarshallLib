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
static unsigned int next_multiple_of_BS(unsigned int, int BS);
static double custom_pow(double base, int exponent);

//----------------------------------------------- Lib Functions -----------------------------------------

FW_Matrix fwl_matrix_create(DataType dataType, char *path, int BS, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = fwl_attr_new();
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

    // Set Block Size
    if (BS != -1)
    {
        FW.BS = BS;
    }
    else
    {
        FW.BS = DEFAULT_BLOCK_SIZE;
    }

    if(FW.size < FW.BS)
    {
        printf("WARNING: The matrix size is smaller than the block size\n");
    }

    FW.norm_size = next_multiple_of_BS(FW.size, FW.BS); //Size normalization          
    create_matrixes_from_file(&FW, file, local_attr.no_path);

    fclose(file);
    FW_creation_time = dwalltime() - timetick_start;
    return FW;
}

void fwl_matrix_paralell_search(FW_Matrix FW, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = fwl_attr_new();
    }
    else
    {
        local_attr = *attr;
    }

    // If thread number is 1, run the sequential version 
    if(local_attr.thread_num == 1)
    {
        fwl_matrix_sequential_search(FW, &local_attr);
        return;
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

void fwl_matrix_sequential_search(FW_Matrix FW, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;
    if (attr == NULL)
    {
        local_attr = fwl_attr_new();
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

void fwl_matrix_save(FW_Matrix FW, char *path, char *name, FileType fileType, FW_attr_t *attr)
{
    double timetick_start = dwalltime();

    FW_attr_t local_attr;

    if (attr == NULL)
    {
        // attr es NULL, usa valores predeterminados
        local_attr = fwl_attr_new();
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

void fwl_matrix_free(FW_Matrix *FW)
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

//----------------------------------------------- Details -----------------------------------------

char* fwl_matrix_get_info(FW_Matrix *element) 
{
    const int buffer_size = 1024;
    int len = 0;
    char *result = malloc(buffer_size);
    if (result == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in FW_details_to_string().\n");
        return "";
    }

    len += snprintf(result + len, buffer_size - len, "-> File format: %s\n", (element->fileType == CSV) ? "CSV" : "JSON");
    len += snprintf(result + len, buffer_size - len, "-> Datatype: %s\n", dataType_to_str(element->datatype));
    len += snprintf(result + len, buffer_size - len, "-> Matrix Size: %d\n", element->size);
    len += snprintf(result + len, buffer_size - len, "-> Matrix Normalized Size: %d\n", element->norm_size);
    len += snprintf(result + len, buffer_size - len, "-> Block Size: %d\n", element->BS);
    if (element->datatype != TYPE_INT) {
        len += snprintf(result + len, buffer_size - len, "-> Decimal Places: %d\n", element->decimal_length);
    }

    return result;
}

char* fwl_attr_get_info(FW_attr_t *attr) 
{

    const int buffer_size = 1024;
    int len = 0;
    char *result = malloc(buffer_size);
    if (result == NULL) {
        fprintf(stderr, "Error: Memory allocation failed in FW_details_to_string().\n");
        return "";
    }

    len += snprintf(result + len, buffer_size - len, "=> %d threads\n", attr->thread_num);
    len += snprintf(result + len, buffer_size - len, "=> %sncluding paths\n", attr->no_path ? "Not i" : "I");
    len += snprintf(result + len, buffer_size - len, "=> Infinite outputs as %s\n", attr->text_in_output ? "INF" : "-1");

    return result;
}

//----------------------------------------------- Attr -----------------------------------------

FW_attr_t fwl_attr_new()
{
    FW_attr_t attr;
    attr.text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr.print_distance_matrix = DEFAULT_PRINT_DIST_MATRIX;
    attr.no_path = DEFAULT_NO_PATH;
    attr.thread_num = DEFAULT_THREAD_NUM;

    return attr;
}

void fwl_attr_init(FW_attr_t *attr)
{
    // Initialize the attributes of the FW_attr_t object
    attr->text_in_output = DEFAULT_OUTPUT_FORMAT;
    attr->print_distance_matrix = DEFAULT_PRINT_DIST_MATRIX;
    attr->no_path = DEFAULT_NO_PATH;
    attr->thread_num = DEFAULT_THREAD_NUM;
}

//----------------------------------------------- Times -----------------------------------------

double fwl_get_create_time()
{
    return FW_creation_time;
}

double fwl_get_search_time()
{
    return FW_processing_time;
}

double fwl_get_save_time()
{
    return FW_save_time;
}

double fwl_get_total_time()
{
    return FW_creation_time + FW_processing_time + FW_save_time;
}

//----------------------------------------------- Performance -----------------------------------------

double get_fw_performance(FW_Matrix * FW)
{
    double performance = 0;

    performance = (double)FW->size * (double)FW->size * (double)FW->size * 2; // *2 porque se realiza la suma y una comparacion.
    return performance / (fwl_get_search_time() * custom_pow(10, 9)); // Mostrar en giga Flops 10^9
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

static unsigned int next_multiple_of_BS(unsigned int n, int BS)
{
    unsigned count = 0;

    int remainder = n % BS;
    if (remainder == 0)
    {
        return n;
    }
    else
    {
        return n + (BS - remainder);
    }
}

static double custom_pow(double base, int exponent)
{
    double result = 1.0;
    int abs_exponent = (exponent < 0) ? -exponent : exponent;

    for (int i = 0; i < abs_exponent; i++) {
        result *= base;
    }

    if (exponent < 0) {
        result = 1.0 / result;
    }

    return result;
}