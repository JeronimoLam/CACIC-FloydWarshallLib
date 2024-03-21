#include "FW_Lib_Functions.h"

#define FW_DEFAULT_BLOCK_SIZE 2
#define FW_DEFAULT_THREAD_NUM 2

void print_matrix(void*, unsigned int, DataType);
char* dataTypeToString(DataType);
unsigned int nextPowerOf2(unsigned int);
//void graphFromRowsToBlocks(FW_Matrix);
//void graphFromBlocksToRows(FW_Matrix FW, int * A );

/**
 * @brief Initializes and loads a matrix from a file, preparing it for the Floyd-Warshall algorithm.
 *
 * This function opens a file specified by 'path', detects the data type and the structure of the matrix,
 * calculates the required normalized size (power of two), and divides the matrix into blocks according to the block
 * size specified by 'BS'. Then, it loads the matrix into memory according to these parameters.
 *
 * @param dataType Data type of the matrix elements. If UNDEFINED is specified, the function will attempt to
 * auto-detect it.
 * @param path Path to the file containing the matrix.
 * @param BS Block size for matrix division. If -1 is passed, the default block size is used.
 * @return FW_Matrix Structure containing the matrix divided into blocks, among other relevant metadata.
 */
FW_Matrix create_structure(DataType dataType, char * path, int BS){
    FW_Matrix FW;
    FILE* file = NULL;

    // Open the file and set file type
    file = getFile(path);
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open the file.\n");
        exit(EXIT_FAILURE);
    }
    FW.fileType = getFileType();
    printf("File Type: %s\n", fileTypeToString());

    // Autodetect of datatype if not specified
    if(dataType == UNDEFINED){
        printf("Auto-Detecting Data Type...\n");
        FW.datatype = AutoDetectDataType(FW.fileType, file);
    }
    else{
        FW.datatype = dataType;
    }

    printf("Selected %s\n", dataTypeToString(FW.datatype));

    // Calculation of matrix size
    calculateMatrixSize(&FW, file); // Calulates rows and cols
    FW.norm_size = nextPowerOf2(FW.size); // Calculates the max size of the matrix (nxn)
    printf("Matrix Size: %d X %d\n", FW.size, FW.size);
    printf("Normalized Size: %d -> %d   (%d X %d)\n", FW.size, FW.norm_size, FW.norm_size, FW.norm_size);

    // Set Block Size
    if (BS != -1){
        FW.BS = BS;
    }
    else{
        FW.BS = FW_DEFAULT_BLOCK_SIZE;
    }
    printf("Block Size: %d\n\n", FW.BS);


    FW.dist = createMatrix(FW, file);
    printf("Matrix ordered by blocks\n");
    print_matrix(FW.dist, FW.norm_size, FW.datatype);

    return FW;
}

/**
 * @brief Performs the Floyd-Warshall algorithm calculation on the provided matrix.
 *
 * This function is expected to implement the logic of the Floyd-Warshall algorithm to calculate the shortest distances
 * between all pairs of vertices in a graph, using the distance matrix contained in 'FW'. It is expected to
 * parallelize the calculations to improve performance.
 *
 * @param FW FW_Matrix structure containing the distance matrix and additional metadata necessary for
 * the calculation.
 * @note This function is pending implementation, and its description may change based on the specific
 * details of the final implementation.
 */
void compute_FW(FW_Matrix FW){

}

/**
 * @brief Saves the distance matrix or the shortest path matrix to a file.
 *
 * This function exports the distance matrix or the path matrix (as specified by the parameters) to a file
 * at the specified path. The function adjusts the file name to prevent overwrites and ensures the file format
 * is correct according to the specified type.
 *
 * @param FW The FW_Matrix structure containing the matrices to be exported.
 * @param path Path to the directory where the exported file will be saved.
 * @param name Base name for the exported file. The function may modify this name to prevent overwrites.
 * @param fileType File type for the export (e.g., binary or text).
 * @param dist_matrix Indicates whether the distance matrix should be exported (1) or not (0).
 * @param path_matrix Indicates whether the shortest path matrix should be exported (1) or not (0).
 * @note If both dist_matrix and path_matrix are set to 0, the function will not perform any action.
 */
void save_structure(FW_Matrix FW, char * path, char * name, FileType fileType, int dist_matrix, int path_matrix){
    if (dist_matrix == 0 & path_matrix == 0){
        printf("Select a matrix to export\n");
        return;
    }

    if (dist_matrix == 1){
        printf("Exporting Distance Matrix...\n");
    }
    else{
        printf("Exporting Path Matrix...\n");
    }

// Create copies of path and name for manipulation
    char pathCopy[1024];
    char nameCopy[256];  // Assuming name length won't exceed 256, adjust as needed
    strncpy(pathCopy, path, sizeof(pathCopy) - 1);
    strncpy(nameCopy, name, sizeof(nameCopy) - 1);
    pathCopy[sizeof(pathCopy) - 1] = '\0';  // Ensure null termination
    nameCopy[sizeof(nameCopy) - 1] = '\0';  // Ensure null termination

    // Remove trailing '/' or '\' from pathCopy
    int pathLength = strlen(pathCopy);
    if (pathCopy[pathLength - 1] == '/' || pathCopy[pathLength - 1] == '\\') {
        pathCopy[pathLength - 1] = '\0';
    }

    // Modify nameCopy to contain content only until the first '.'
    char *dotPosition = strchr(nameCopy, '.');
    if (dotPosition != NULL) {
        *dotPosition = '\0';
    }

    char fullPath[1024];  // Buffer for full path
    sprintf(fullPath, "%s/%s", pathCopy, nameCopy);
    printf("Full Path: %s\n", fullPath);

    saveMatrix(FW, fullPath, fileType, dist_matrix, path_matrix);
}


//AUX Functions
void print_matrix(void* matrix, unsigned int size, DataType dataType) {
    int i, j;
    switch (dataType) {
        case TYPE_INT:
            for (i = 0; i < size; i++) {
                for (j = 0; j < size; j++) {
                    printf("%d ", ((int *) matrix)[i * size + j]);
                }
                printf("\n");
            }
            break;
        case TYPE_FLOAT:
            for (i = 0; i < size; i++) {
                for (j = 0; j < size; j++) {
                    printf("%f ", ((float *) matrix)[i * size + j]);
                }
                printf("\n");
            }
            break;
        case TYPE_DOUBLE:
            for (i = 0; i < size; i++) {
                for (j = 0; j < size; j++) {
                    printf("%lf ", ((double *) matrix)[i * size + j]);
                }
                printf("\n");
            }
            break;
        case TYPE_CHAR:
            for (i = 0; i < size; i++) {
                for (j = 0; j < size; j++) {
                    printf("%c ", ((char *) matrix)[i * size + j]);
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

char* dataTypeToString(DataType dt) {
    const char *prefix = "Data Type: ";
    char *result = malloc(30);  // allocate enough memory for the prefix and the datatype string
    switch (dt) {
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

unsigned int nextPowerOf2(unsigned int n) {
    unsigned count = 0;

    // First n in the below condition is for the case where n is 0
    if (n && !(n & (n - 1))) {
        return n;
    }

    while (n != 0) {
        n >>= 1;
        count += 1;
    }

    return 1 << count;
}

/*
void graphFromRowsToBlocks(FW_Matrix FW){
    int * B = (int *) FW.dist, * A = malloc(FW.norm_size * FW.norm_size * sizeof(int));

    unsigned int I,J,i,j,blockSize,r;
    r = FW.norm_size / FW.BS;

    blockSize = FW.BS * FW.BS;
    for(I=0; I<r; I++)
        for(J=0; J<r; J++)
            for(i=0; i < FW.BS; i++)
                for(j=0; j < FW.BS; j++)
                    A[I * FW.norm_size * FW.BS + J * blockSize + i * FW.BS + j] = B[I * FW.norm_size * FW.BS + J * FW.BS + i * FW.norm_size + j];
    print_matrix(A, FW.norm_size, TYPE_INT);
    //graphFromBlocksToRows(FW, A);
}

void graphFromBlocksToRows(FW_Matrix FW, int * A ){
    int * B = malloc(FW.norm_size * FW.norm_size * sizeof(int));

    unsigned int I,J,i,j,blockSize,r;
    r = FW.norm_size / FW.BS;

    blockSize = FW.BS * FW.BS;
    for(I=0; I<r; I++)
        for(J=0; J<r; J++)
            for(i=0; i < FW.BS; i++)
                for(j=0; j < FW.BS; j++)
                    B[I * FW.norm_size * FW.BS + J * FW.BS + i * FW.norm_size + j] = A[I * FW.norm_size * FW.BS + J * blockSize + i * FW.BS + j];
}
 */