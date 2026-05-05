#!/bin/bash

./clean_bld.sh

cd ../src

make clean
make build
./bld/tst_experiment.exe
cp ./bld/experiment.exe ../experiment/bld/experiment.exe

cd ../experiment

echo "Experiment executable file is set"