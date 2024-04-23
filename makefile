
OS ?= Linux
LIB_NAME = FloydWarshall
OBJ_DIR = obj
LIB_DIR = lib

ifeq ($(OS),Windows_NT)
all: prepare $(LIB_DIR)/lib$(LIB_NAME)_static.a ./lib$(LIB_NAME)_dynamic.dll
CFLAGS = -O0 -g -fopenmp
else
all: prepare $(LIB_DIR)/lib$(LIB_NAME)_static.a ./$(LIB_DIR)/lib$(LIB_NAME)_dynamic.so

CFLAGS = -O0 -g -fopenmp -fPIC
endif

clean:
ifeq ($(OS),Windows_NT)
	@cmd /c "echo Clearing the screen... && cls"
else
	@echo "Clearing the screen..."
	@clear
endif

prepare:
ifeq ($(OS),Windows_NT)
	@if not exist "$(OBJ_DIR)" mkdir $(OBJ_DIR)
	@if not exist "$(LIB_DIR)" mkdir $(LIB_DIR)
else
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)
endif


OBJECTS = $(OBJ_DIR)/file_handler.o $(OBJ_DIR)/blocks.o $(OBJ_DIR)/common_functions.o $(OBJ_DIR)/CSV_createMatrix.o $(OBJ_DIR)/CSV_Helpers.o $(OBJ_DIR)/CSV_saveMatrix.o $(OBJ_DIR)/JSON_CreateMatrix.o $(OBJ_DIR)/JSON_Helpers.o $(OBJ_DIR)/JSON_saveMatrix.o $(OBJ_DIR)/FW_Lib_Functions.o $(OBJ_DIR)/FW_compute_int.o $(OBJ_DIR)/FW_compute_float.o $(OBJ_DIR)/FW_compute_double.o

$(OBJ_DIR)/file_handler.o: Floyd_Warshall_Lib/File_Manager/file_handler.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/CSV_createMatrix.o: Floyd_Warshall_Lib/File_Manager/CSV/CSV_createMatrix.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/CSV_Helpers.o: Floyd_Warshall_Lib/File_Manager/CSV/CSV_Helpers.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/CSV_saveMatrix.o: Floyd_Warshall_Lib/File_Manager/CSV/CSV_saveMatrix.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/JSON_CreateMatrix.o: Floyd_Warshall_Lib/File_Manager/JSON/JSON_CreateMatrix.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/JSON_Helpers.o: Floyd_Warshall_Lib/File_Manager/JSON/JSON_Helpers.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/JSON_saveMatrix.o: Floyd_Warshall_Lib/File_Manager/JSON/JSON_saveMatrix.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/blocks.o: Floyd_Warshall_Lib/File_Manager/Block_Operations/blocks.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/common_functions.o: Floyd_Warshall_Lib/File_Manager/Common/common_functions.c
	gcc $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/FW_Lib_Functions.o: Floyd_Warshall_Lib/FW_Lib_Functions.c
	gcc $(CFLAGS) -c -fopenmp $< -o $@ -Iinclude

$(OBJ_DIR)/FW_compute_int.o: Floyd_Warshall_Lib/FW_compute_int.c
	gcc $(CFLAGS) -c -fopenmp $< -o $@ -IFloyd_Warshall_Lib/

$(OBJ_DIR)/FW_compute_float.o: Floyd_Warshall_Lib/FW_compute_float.c
	gcc $(CFLAGS) -c -fopenmp $< -o $@ -IFloyd_Warshall_Lib/

$(OBJ_DIR)/FW_compute_double.o: Floyd_Warshall_Lib/FW_compute_double.c
	gcc $(CFLAGS) -c -fopenmp $< -o $@ -IFloyd_Warshall_Lib/

$(LIB_DIR)/lib$(LIB_NAME)_static.a: $(OBJECTS)
	ar rcs $@ $(OBJECTS)

ifeq ($(OS),Windows_NT)
./lib$(LIB_NAME)_dynamic.dll: $(OBJECTS)
	gcc -DBUILDING_DLL -O0 -fopenmp -shared -o $@ $(OBJECTS) -L$(LIB_DIR) -lFloydWarshall_static -Wl,--out-implib,$(LIB_DIR)/lib$(LIB_NAME)_dynamic.dll.a -Wl,--output-def,$(LIB_DIR)/$(LIB_NAME).def
else
./$(LIB_DIR)/lib$(LIB_NAME)_dynamic.so: $(OBJECTS)
	gcc -DBUILDING_DLL -O0 -fopenmp -shared -fPIC -o $@ $(OBJECTS) -L$(LIB_DIR) -lFloydWarshall_static -Wl,-soname,$(LIB_NAME)_dynamic.so

endif
.PHONY: all clean prepare
