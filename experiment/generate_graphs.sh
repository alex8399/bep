#!/bin/bash

./clean_graphs.sh

vertices_numbers=(100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000)
edge_to_vertice_factor=12
min_weight=0
max_weight=500

for vertices_number in "${vertices_numbers[@]}"
do
    edges_number=$((edge_to_vertice_factor * vertices_number))
    file_path="./graphs/graph_${vertices_number}.txt"

    echo "Generate graph with ${vertices_number} vertices"
    python3 generate_graph.py ${vertices_number} ${edges_number} ${min_weight} ${max_weight} ${file_path}
    echo "Graph is generated and saved into ${file_path}"
done
