#include "../include/FW_Lib_Functions.h"
#include "File_Manager/file_handler.h"

#include <omp.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define FW_DEFAULT_BLOCK_SIZE 2
#define FW_DEFAULT_THREAD_NUM 2
#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#define NO_PATH

// Private functions
void print_matrix(void *, unsigned int, DataType);
char *dataTypeToString(DataType);
unsigned int nextPowerOf2(unsigned int);
static inline void FW_BLOCK_PARALLEL(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2) __attribute__((always_inline));
static inline void FW_BLOCK(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2) __attribute__((always_inline));

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
    printf("File Type: %s\n", fileTypeToString());

    // Autodetect of datatype if not specified
    if (dataType == UNDEFINED)
    {
        printf("Auto-Detecting Data Type...\n");
        FW.datatype = AutoDetectDataType(FW.fileType, file);
    }
    else
    {
        FW.datatype = dataType;
    }

    printf("Selected %s\n", dataTypeToString(FW.datatype));

    // Calculation of matrix size
    calculateMatrixSize(&FW, file);       // Calulates rows and cols
    FW.norm_size = nextPowerOf2(FW.size); // Calculates the max size of the matrix (nxn)
    printf("Matrix Size: %d X %d\n", FW.size, FW.size);
    printf("Normalized Size: %d -> %d   (%d X %d)\n", FW.size, FW.norm_size, FW.norm_size, FW.norm_size);

    // Set Block Size
    if (BS != -1)
    {
        FW.BS = BS;
    }
    else
    {
        FW.BS = FW_DEFAULT_BLOCK_SIZE;
    }
    printf("Block Size: %d\n\n", FW.BS);

    FW.dist = createMatrix(FW, file); // TODO: Revisar tema de espacio en memoria al pasar el FW como parametro. Se duplican las matrices?
    printf("Matrix ordered by blocks\n");

    // FW.path = createMatrix(FW, file); // TODO: implementacion de la inicializacion de la matriz de caminos

    printf("Matriz de caminos inicailizada\n");
    print_matrix(FW.dist, FW.norm_size, FW.datatype);

    return FW;
}

void compute_FW(FW_Matrix FW)
{
    uint64_t r, row_of_blocks_disp, num_of_bock_elems;
    r = FW.norm_size / FW.BS;
    row_of_blocks_disp = FW.norm_size * FW.BS;
    num_of_bock_elems = FW.BS * FW.BS;
    int BS = FW.BS;

    // --------------------------- BLOQUE AGREGADO -----------------------

    uint64_t x, y;
    sem_t **S;

    // asignación de memoria
    S = (sem_t **)malloc(r * sizeof(sem_t *));
    for (x = 0; x < r; x++)
        S[x] = (sem_t *)malloc(r * sizeof(sem_t));

    for (x = 0; x < r; x++)
    {
        for (y = 0; y < r; y++)
        {
            sem_init(&S[x][y], 0, 0);
        }
    }

    // ------------------------- FIN BLOQUE AGREGADO -----------------------

    // Modificación: shared(pendientes, cv, mutex)

#pragma omp parallel shared(S) default(none) firstprivate(r, row_of_blocks_disp, num_of_bock_elems, D, P) num_threads(t)
    {
        uint64_t i, j, k, b, kj, ik, kk, ij, k_row_disp, k_col_disp, i_row_disp, j_col_disp, w;

        // Variable agregada
        uint64_t aux;

#ifndef NO_PATH
        int *tmp1 = NULL;
        int *tmp2 = NULL;
#else
        int tmp1[BS], tmp2[BS];
        // tmp1 and tmp2 are used as a patch to avoid a compiler bug which makes it lose performance instead of winning while omitting the compute of the P patrix
#endif

        // inicialización de semaforos
        // #pragma omp for collapse(2)

        for (k = 0; k < r; k++)
        {
            b = k * FW.BS;
            k_row_disp = k * row_of_blocks_disp;
            k_col_disp = k * num_of_bock_elems;

            // Phase 1
            kk = k_row_disp + k_col_disp;
            FW_BLOCK_PARALLEL(FW.dist, FW.BS, kk, kk, kk, FW.path, b, tmp1, tmp2);

// Phase 2 y 3
#pragma omp for schedule(dynamic) nowait
            for (w = 0; w < r * 2; w++)
            {
                if (w < r)
                {
                    // Phase 2
                    j = w;
                    if (j == k)
                        continue;
                    kj = k_row_disp + j * num_of_bock_elems;
                    FW_BLOCK(FW.dist, FW.BS, kj, kk, kj, FW.path, b, tmp1, tmp2);

                    // -------------- BLOQUE AGREGADO -------------------

                    // Finalizó el computo del bloque (k,j) = (k,w)
                    // Modif: se debe decrementar pendientes de la columna actual "j"
                    for (aux = 0; aux < r; aux++)
                    {
                        if (aux == k)
                            continue; // se ignora actual (k,j)
                        sem_post(&S[aux][j]);
                    }

                    // -------------- FIN BLOQUE AGREGADO -----------------
                }
                else
                {
                    // Phase 3
                    i = w - r;
                    if (i == k)
                        continue;
                    ik = i * row_of_blocks_disp + k_col_disp;
                    FW_BLOCK(FW.dist, FW.BS, ik, ik, kk, FW.path, b, tmp1, tmp2);

                    // -------------- BLOQUE AGREGADO -------------------

                    // Finalizo el computo del bloque (i,k) = (w-r, k)
                    // Modif: se debe decrementar pendientes de la fila actual "i"
                    for (aux = 0; aux < r; aux++)
                    {
                        if (aux == k)
                            continue; // se ignora actual (i,k)
                        sem_post(&S[i][aux]);
                    }

                    // -------------- FIN BLOQUE AGREGADO -----------------
                }
            }

// Phase 4
#pragma omp for collapse(2) schedule(dynamic)
            for (i = 0; i < r; i++)
            {
                for (j = 0; j < r; j++)
                {
                    if ((j == k) || (i == k))
                        continue;

                    // ----------- BLOQUE AGREGADO -----------------

                    // Esperar que se computen los bloques (k,j) e (i,k)
                    sem_wait(&S[i][j]);
                    sem_wait(&S[i][j]);

                    // ---------- FIN BLOQUE AGREGADO --------------

                    i_row_disp = i * row_of_blocks_disp;
                    ik = i_row_disp + k_col_disp;
                    j_col_disp = j * num_of_bock_elems;
                    kj = k_row_disp + j_col_disp;
                    ij = i_row_disp + j_col_disp;
                    FW_BLOCK(FW.dist, FW.BS, ij, ik, kj, FW.path, b, tmp1, tmp2);
                }
            }
        }
    }

    // liberación de memoria reservada
    for (x = 0; x < r; x++)
        free(S[x]);
    free(S);
}

void save_structure(FW_Matrix FW, char *path, char *name, FileType fileType, int dist_matrix, int path_matrix)
{
    if (dist_matrix == 0 & path_matrix == 0)
    {
        printf("Select a matrix to export\n");
        return;
    }

    if (dist_matrix == 1)
    {
        printf("Exporting Distance Matrix...\n");
    }
    else
    {
        printf("Exporting Path Matrix...\n");
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

    char fullPath[1024]; // Buffer for full path
    sprintf(fullPath, "%s/%s", pathCopy, nameCopy);
    printf("Full Path: %s\n", fullPath);

    saveMatrix(FW, fullPath, fileType, dist_matrix, path_matrix);
}

char *FW_details_to_string(FW_Matrix element, int dist, int path)
{
    char *result = malloc(1024);
    sprintf(result, "Matrix Size: %d\nNormalized Size: %d\nBlock Size: %d\nData Type: %s\n", element.size, element.norm_size, element.BS, dataTypeToString(element.datatype));
    // if (dist == 1)
    // {
    //     printf("Distance Matrix Loaded\n");
    //     print_matrix(element.dist, element.norm_size, element.datatype);
    // }
    // if (path == 1)
    // {
    //     printf("Path Matrix\n");
    //     print_matrix(element.path, element.norm_size, TYPE_INT);
    // }
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

// Paralell algorithm implementation
#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2)
{
    // Casteo de graph a int
    int *graph_int = (int *)graph; // Segun Datatype

    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    uint64_t dij, dik, dkj, sum; // Segun Datatype

    for (k = 0; k < BS; k++)
    {
        k_disp = k * BS;
        k_disp_d3 = k_disp + d3;
        for (i = 0; i < BS; i += 2)
        {
            i_disp = i * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                dij = graph_int[i_disp_d1 + j];
                dkj = graph_int[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
            i_disp = (i + 1) * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                dij = graph_int[i_disp_d1 + j];
                dkj = graph_int[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
        }
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static inline void FW_BLOCK_PARALLEL(void *const graph, int BS, const uint64_t d1, const uint64_t d2, const uint64_t d3, int *const path, const uint64_t base, int *const tmp1, int *const tmp2)
{
    // Casteo de graph a int
    int *graph_int = (int *)graph; // Segun Datatype

    uint64_t i, j, k, i_disp, i_disp_d1, k_disp, k_disp_d3;
    uint64_t dij, dik, dkj, sum; // Segun Datatype

    for (k = 0; k < BS; k++)
    {
        k_disp = k * BS;
        k_disp_d3 = k_disp + d3;
#pragma omp for
        for (i = 0; i < BS; i += 2)
        {
            i_disp = i * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                int test1 = graph_int[i_disp_d1 + j], test2 = graph_int[k_disp_d3 + j];
                dij = (uint64_t)test1;
                dkj = (uint64_t)test2;
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
            i_disp = (i + 1) * BS;
            i_disp_d1 = i_disp + d1;
            dik = graph_int[i_disp + d2 + k];
#ifdef INTEL_ARC
#pragma unroll(UNROLL_FACTOR)
#endif
#pragma omp simd private(dij, dkj, sum)
            for (j = 0; j < BS; j++)
            {
                dij = graph_int[i_disp_d1 + j];
                dkj = graph_int[k_disp_d3 + j];
                sum = dik + dkj;
                if (unlikely(sum < dij))
                {
                    graph_int[i_disp_d1 + j] = sum;
#ifndef NO_PATH
                    path[i_disp_d1 + j] = base + k;
#else
                    tmp1[j] = tmp2[j];
#endif
                }
            }
        }
    }
}

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
    const char *prefix = "Data Type: ";
    char *result = malloc(30); // allocate enough memory for the prefix and the datatype string
    switch (dt)
    {
    case TYPE_INT:
        sprintf(result, "%s%s", prefix, "INT");
        break;
    case TYPE_FLOAT:
        sprintf(result, "%s%s", prefix, "FLOAT");
        break;
    case TYPE_DOUBLE:
        sprintf(result, "%s%s", prefix, "DOUBLE");
        break;
    case TYPE_CHAR:
        sprintf(result, "%s%s", prefix, "CHAR");
        break;
    case UNDEFINED:
    default:
        sprintf(result, "%s%s", prefix, "unknown");
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
