@echo off
setlocal enabledelayedexpansion

set algorithm_types=^
    SEQ-FINDPIVOT SEQ-FINDPIVOTSAFELY ^
    OPENMP-MAPREDUCE OPENMP-MUTEX ^
    STDTHREAD-MAPREDUCE STDTHREAD-MUTEX ^
    CUDA ^
    OPENCL-NVIDIAGPU OPENCL-CPU
set vertices_numbers=100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000
set run_number=50

set OMP_NUM_THREADS=12
set OMP_WAIT_POLICY=active
set GOMP_SPINCOUNT=infinite

for %%A in (%algorithm_types%) do (
    for %%V in (%vertices_numbers%) do (
        set graph_file_path=.\graphs\graph_%%V.txt
        set result_file_path=.\results\%%A_%%V.txt
        set experiment_exe_path=.\bld\experiment.exe

        echo Run experiment %run_number% times for algorithm %%A on graph with %%V vertices
        
        if %%A==CUDA (
            .\bld\experiment_nvcc.exe %%A %run_number% !graph_file_path! !result_file_path!
        ) else if %%A==OPENMP-MUTEX (
            start /affinity 3F /b /wait .\bld\experiment.exe %%A %run_number% !graph_file_path! !result_file_path!
        ) else (
            .\bld\experiment.exe %%A %run_number% !graph_file_path! !result_file_path!
        )

        echo Experiment is finished and saved into !result_file_path!
    )
)