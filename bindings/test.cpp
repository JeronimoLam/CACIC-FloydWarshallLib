#include <iostream>
#include <dlfcn.h>
#include "include/FW_Lib_CommonTypes.h"
#include "include/FW_Lib_Functions.h"

int main() {
    void* handle;
    char* error;

    // Open the dynamic library
    handle = dlopen("lib/libFloydWarshall_dynamic.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        return 1;
    }

    // Clear any existing errors
    dlerror();

    // Function pointers declaration
    auto create_structure = (FW_Matrix (*)(DataType, char*, int, FW_attr_t*)) dlsym(handle, "create_structure");
    auto FW_details_to_string = (char* (*)(FW_Matrix*, FW_attr_t*)) dlsym(handle, "FW_details_to_string");
    auto compute_FW_parallel = (void (*)(FW_Matrix, FW_attr_t*)) dlsym(handle, "compute_FW_parallel");
    auto save_structure = (void (*)(FW_Matrix, const char*, const char*, FileType, FW_attr_t*)) dlsym(handle, "save_structure");
    auto free_FW_Matrix = (void (*)(FW_Matrix*)) dlsym(handle, "free_FW_Matrix");
    auto get_FW_processing_time = (double (*)()) dlsym(handle, "get_FW_processing_time");

    // Check for errors in loading symbols
    if ((error = dlerror()) != nullptr) {
        std::cerr << "Error loading symbols: " << error << '\n';
        dlclose(handle);
        return 1;
    }

    // Use the functions
    FW_attr_t attr = {1, 1, 0, 4};  // Sample attributes
    FW_Matrix data = create_structure(TYPE_FLOAT, "examples/3.csv", 2, &attr);

    std::cout << FW_details_to_string(&data, &attr) << "\n ==> Procesado \n";
    print_FW_matrixes(&data, "dist", 0);
    compute_FW_parallel(data, &attr);

    std::cout << " ==> Guardando \n";
    save_structure(data, "./output/", "Resultparallel.csv", CSV, &attr);

    free_FW_Matrix(&data); // Free memory

    double parallel_algorithm_time = get_FW_processing_time();
    double parallel_total_time = get_FW_processing_time(); // Should this be another function?

    std::cout << "Tiempo algoritmo Paralelo: " << parallel_algorithm_time << " segundos\n";
    std::cout << "Tiempo total Paralelo: " << parallel_total_time << " segundos\n";

    // Close the library
    dlclose(handle);
    return 0;
}
