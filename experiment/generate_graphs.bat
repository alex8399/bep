@echo off
setlocal enabledelayedexpansion

call .\clean_graphs.bat

set edge_to_vertice_factor=12
set min_weight=0
set max_weight=500
set vertices_numbers=100000 200000 300000 400000 500000 600000 700000 800000 900000 1000000

for %%v in (%vertices_numbers%) do (
    set /a edges_number = %edge_to_vertice_factor% * %%v
    set file_path=.\graphs\graph_%%v.txt

    echo Generate graph with %%v vertices
    python3 generate_graph.py %%v !edges_number! %min_weight% %max_weight% !file_path!
    echo Graph is generated and saved into !file_path!
)