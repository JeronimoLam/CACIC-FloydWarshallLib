@echo off
REM Compilation script for the Generator project

REM Compile all C source files and link them into a single executable
gcc -O0 -g src/DoubleGenerator.c src/FloatGenerator.c src/IntGenerator.c ExampleGenerator.c -o GeneratorExecutable

echo Compilation and linking complete!
