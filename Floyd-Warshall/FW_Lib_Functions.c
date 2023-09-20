#include "FW_Lib_Functions.h"

#define FW_DEFAULT_BLOCK_SIZE 2
#define FW_DEFAULT_THREAD_NUM 2

void print_matrix(void*, unsigned int, DataType);
char* dataTypeToString(DataType);
unsigned int nextPowerOf2(unsigned int);
//void graphFromRowsToBlocks(FW_Matrix);
//void graphFromBlocksToRows(FW_Matrix FW, int * A );

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

void compute_FW(FW_Matrix * FW){

}
void save_structure(FW_Matrix FW, char * path,int dist_matrix, int path_matrix){
    if (dist_matrix == 0 & path_matrix == 0){
        printf("Select a matrix to export\n");
        return;
    }
    saveMatrix(FW, path, dist_matrix, path_matrix);
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