# Verificación de variable requerida
OS ?= UNDEFINED
FLAGS = -O3 -Wall

all: check-os clean compile

check-os:
ifeq (,$(filter windows linux,$(OS)))
	$(error Invalid OS value '$(OS)'. Please set OS to either 'windows' or 'linux'.)
endif

compile:
ifeq ($(OS),windows)
	@echo "==> Building the generator for Windows"
	@gcc $(FLAGS) src/DecimalGenerator.c src/IntegerGenerator.c ExampleGenerator.c -o GeneratorExecutable.exe
else
	@echo "==> Building the generator for Linux"
	@gcc $(FLAGS) src/DecimalGenerator.c src/IntegerGenerator.c ExampleGenerator.c -o GeneratorExecutable.bin
endif
	@echo "-> Compilation Done!"

clean:
ifeq ($(OS),windows)
	@-if exist *.exe del /Q *.exe
else
	@rm -f *.bin
endif
