@echo off

call .\clean_bld.bat

cd ..\src

make clean
make build
make run_tests
copy .\bld\experiment.exe ..\experiment\bld\experiment.exe
copy .\bld\experiment_nvcc.exe ..\experiment\bld\experiment_nvcc.exe

cd ..\experiment

echo Experiment executable file is set