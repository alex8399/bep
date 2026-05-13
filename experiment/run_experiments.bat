@echo off
setlocal enabledelayedexpansion

set algorithm_types=SEQ-FINDPIVOT SEQ-FINDPIVOTSAFELY OPENMP-MAPREDUCE STDTHREAD CUDA OPENCL-NVIDIAGPU OPENCL-INTELGPU OPENCL-CPU
set vertices_numbers=100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000
set run_number=30

set threads_number=12
set OMP_NUM_THREADS=%threads_number%
set OMP_THREAD_LIMIT=%threads_number%

for %%A in (%algorithm_types%) do (
    for %%V in (%vertices_numbers%) do (
        set graph_file_path=.\graphs\graph_%%V.txt
        set result_file_path=.\results\%%A_%%V.txt
        set experiment_exe_path=.\bld\experiment.exe
        if %%A==CUDA (
            set experiment_exe_path=.\bld\experiment_nvcc.exe
        )

        echo Run experiment %run_number% times for algorithm %%A on graph with %%V vertices
        "!experiment_exe_path!" %%A %run_number% !graph_file_path! !result_file_path!
        echo Experiment is finished and saved into !result_file_path!
    )
)