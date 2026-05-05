import networkx as nx
import random
import sys


WEIGHT = 'weight'


def generate_graph(vertices_number: int, edges_number: int,
                   min_weight: int, max_weight: int, path_for_output: str) -> None:

    graph = nx.gnm_random_graph(vertices_number, edges_number)

    components = list(nx.connected_components(graph))

    for ind in range(len(components) - 1):
        source = random.choice(list(components[ind]))
        target = random.choice(list(components[ind+1]))
        graph.add_edge(source, target)

    assert nx.is_connected(graph), "Graph is not connected"

    for source, target in graph.edges():
        graph[source][target][WEIGHT] = random.randint(min_weight, max_weight)

    with open(path_for_output, "w") as file:
        file.write(f"{graph.number_of_nodes()} {graph.number_of_edges()}\n")

        for source, target in graph.edges():
            file.write(f"{source} {target} {graph[source][target][WEIGHT]}\n")


if __name__ == "__main__":

    if len(sys.argv) != 6:
        raise Exception(
            "Incorrect number of arguments\n"
            "Correct usage: python generate_graph.py "
            "vertices_number approximate_number_of_edges min_weight max_weight path_for_output")

    generate_graph(
        int(sys.argv[1]), int(sys.argv[2]),
        int(sys.argv[3]), int(sys.argv[4]), sys.argv[5])
