# Makefile en ./apps/

# Valor por defecto de LINK_TYPE
LINK_TYPE ?= default
OS ?= UNDEFINED
BS ?= 128

all: check-os check-BS build_lib clean create_output_dir compile exit

check-os:
ifeq (,$(filter windows linux,$(OS)))
$(error Invalid OS value '$(OS)'. Please set OS to either 'windows' or 'linux'.)
endif

# Comprobación de que BS es un número
# Comprobación de que BS es un número
check-BS:
ifeq (, $(BS))
$(error BS no puede estar vacio)
endif
ifeq ($(OS),windows)
VALID_NUMBER := $(shell powershell -Command "[regex]::Match('$(BS)', '^[0-9]+$$').Success")
ifneq ($(strip $(VALID_NUMBER)), True)
$(error BS debe ser un numero)
endif
else
VALID_NUMBER=$(shell echo $(BS) | grep -E '^[0-9]+$$')
ifneq ($(strip $(VALID_NUMBER)), $(strip $(BS)))
$(error BS debe ser un numero)
endif
endif

build_lib:
ifeq ($(OS),windows)
	@echo "==> Building the library for Windows"
	@$(MAKE) -C .. all OS=windows BS=$(BS)
else
	@echo "==> Building the library for Linux"
	@$(MAKE) -C .. all OS=linux BS=$(BS)
endif
	@echo "-> Makefile Done!"

clean:
ifeq ($(OS),windows)
	@-if exist *.dll del /Q *.dll
	@-if exist *.so del /Q *.so
	@-if exist *.so del /Q *.exe
else
	@rm -f *.so
	@rm -f *.dll
	@rm -f *.bin
endif

# Verifica el valor de LINK_TYPE y llama a la regla correspondiente
compile:
ifeq ($(LINK_TYPE),static)
	$(MAKE) static
else ifeq ($(LINK_TYPE),dynamic)
	$(MAKE) dynamic
else
	$(MAKE) default
endif

create_output_dir:
ifeq ($(OS),windows)
	@if not exist output (mkdir output)
else
	@mkdir -p output
endif

static:
	@echo "-> Static linking"
ifeq ($(OS),windows)
	gcc -march=native -O3 -fopenmp FW_app.c -o FW_app.exe -L../lib -lFloydWarshall_static
else
	gcc -march=native -O3 -fopenmp FW_app.c -o FW_app.bin -L../lib -lFloydWarshall_static
endif
	@echo "-> Static Lib compilation Done!"

dynamic:
	@echo "-> Dynamic linking"
ifeq ($(OS),windows)
	@copy ..\lib\libFloydWarshall_dynamic.dll libFloydWarshall_dynamic.dll
	@gcc -DBUILDING_DLL -O3 -fopenmp -march=native FW_app.c -o FW_app.exe -L./ -lFloydWarshall_dynamic
else
	@cp ../lib/libFloydWarshall_dynamic.so ./libFloydWarshall_dynamic.so
	@gcc -DBUILDING_DLL -march=native -O3 -fopenmp FW_app.c -o FW_app.bin -L./ -lFloydWarshall_dynamic -Wl,-rpath,$(PWD)
	@mv ./libFloydWarshall_dynamic.so ./FloydWarshall_dynamic.so
endif
	@echo "-> Dynamic Lib compilation Done!"

default:
	@echo "-> DEFAULT Static linking"
	@$(MAKE) static

exit:
	@echo "=> Compilation finished <="

.PHONY: all build_lib compile static dynamic default
