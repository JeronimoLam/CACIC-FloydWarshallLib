#!/bin/bash

./FW.bin -p ./examples/1000.csv -b 128 -t 1 -i >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 2 -i >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 4 -i >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 8 -i >> metricas.txt
./FW.bin -p ./examples/1000.csv -b 128 -t 16 -i >> metricas.txt

echo "----------------------------------------" >> metricas.txt

./FW.bin -p ./examples/2000.csv -b 128 -t 1 -i >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 2 -i >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 4 -i >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 8 -i >> metricas.txt
./FW.bin -p ./examples/2000.csv -b 128 -t 16 -i >> metricas.txt

echo "----------------------------------------" >> metricas.txt

./FW.bin -p ./examples/4000.csv -b 128 -t 1 -i >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 2 -i >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 4 -i >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 8 -i >> metricas.txt
./FW.bin -p ./examples/4000.csv -b 128 -t 16 -i >> metricas.txt
