#!/bin/bash

algorithm_types=("SEQ-FINDPIVOT" "SEQ-FINDPIVOTSAFELY" "OPENMP-MAPREDUCE" "OPENMP-MUTEX" "STDTHREAD" "CUDA")
vertices_numbers=(100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000)
run_number=100

threads_number=12

export OMP_NUM_THREADS=${threads_number}
export OMP_THREAD_LIMIT=${threads_number}

for algorithm_type in "${algorithm_types[@]}"
do
    for vertices_number in "${vertices_numbers[@]}"
    do
        graph_file_path="./graphs/graph_${vertices_number}.txt"
        result_file_path="./results/${algorithm_type}_${vertices_number}.txt"
        
        echo "Run experiment ${run_number} times for algorithm ${algorithm_type} on graph with ${vertices_number} vertices"
        ./bld/experiment.exe ${algorithm_type} ${run_number} ${graph_file_path} ${result_file_path}
        echo "Experiment is finished and saved into ${result_file_path}"
    done
done
