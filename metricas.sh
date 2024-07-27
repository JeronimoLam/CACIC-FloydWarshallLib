#!/bin/bash

# Ejecutar FW.bin con archivos originales y bandera -i
./FW.bin -p ./examples/1000.csv -b 128 -t 1 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 2 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 4 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 8 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 16 -i >> metricas.txt

echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt

./FW.bin -p ./examples/2000.csv -b 128 -t 1 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 2 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 4 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 8 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 16 -i >> metricas.txt

echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt

./FW.bin -p ./examples/4000.csv -b 128 -t 1 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 2 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 4 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 8 -i >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 16 -i >> metricas.txt

echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt

# Ejecutar FW.bin con archivos modificados y bandera -f
./FW.bin -p ./examples/1000f.csv -b 128 -t 1 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000f.csv -b 128 -t 2 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000f.csv -b 128 -t 4 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000f.csv -b 128 -t 8 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/1000f.csv -b 128 -t 16 -f >> metricas.txt

echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt

./FW.bin -p ./examples/2000f.csv -b 128 -t 1 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000f.csv -b 128 -t 2 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000f.csv -b 128 -t 4 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000f.csv -b 128 -t 8 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/2000f.csv -b 128 -t 16 -f >> metricas.txt

echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt

./FW.bin -p ./examples/4000f.csv -b 128 -t 1 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000f.csv -b 128 -t 2 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000f.csv -b 128 -t 4 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000f.csv -b 128 -t 8 -f >> metricas.txt
echo "----------------------------------------" >> metricas.txt
./FW.bin -p ./examples/4000f.csv -b 128 -t 16 -f >> metricas.txt

echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
echo "----------------------------------------" >> metricas.txt
