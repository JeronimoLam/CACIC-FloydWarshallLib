@echo off
REM Compilation script for the Generator project

REM Compile all C source files and link them into a single executable
gcc -O0 -g src/DecimalGenerator.c src/IntegerGenerator.c ExampleGenerator.c -o GeneratorExecutable

echo Compilation and linking complete!
