import networkx as nx
import random

def generate_random_graph(num_nodes, edge_probability):
    # Generate a small random graph
    G = nx.erdos_renyi_graph(n=num_nodes, p=edge_probability, directed=False)  

    for u, v in G.edges():
        G[u][v]["weight"] = round(random.uniform(1, 10), 2)  # Assign random weights between 1 and 10

    # Save the graph to a file in edge list format
    with open("graph.txt", "w") as file:
        file.write(f"{num_nodes}\n")
        for u, v, data in G.edges(data=True):
            file.write(f"{u} {v} {data['weight']}\n")
    print("Graph saved to graph.txt")
    
    return G

def construct_constant_degree_graph(G):
    """
    Constructs G' from G such that each vertex in G' has degree at most 3.
    :param G: A weighted undirected graph (NetworkX Graph or DiGraph)
    :return: The transformed graph G'
    """
    G_prime = nx.Graph()  # G' will be an undirected graph
    
    # Iterate through all nodes in G
    for v in G.nodes():
        neighbors = list(G.neighbors(v))
        num_neighbors = len(neighbors)
        
        # Create a cycle of |N(v)| vertices for v
        cycle_nodes = [f"{v}_{w}" for w in neighbors]  # Unique labels for the cycle nodes
        if num_neighbors == 1:
            G_prime.add_node(cycle_nodes[0])
            continue # Skip if v has only one neighbor
        for i in range(num_neighbors):
            G_prime.add_edge(cycle_nodes[i], cycle_nodes[(i + 1) % num_neighbors], weight=0)  # Zero-weight cycle

    # Add edges for each edge in G
    for u, v, data in G.edges(data=True):
        weight = data.get("weight", 1)  # Default weight is 1 if not provided
        G_prime.add_edge(f"{u}_{v}", f"{v}_{u}", weight=weight)  # Connect corresponding cycle nodes
    
    next_node_id = 0
    node_mapping = {}
    # Save the graph to a file in edge list format
    with open("constant_degree_graph.txt", "w") as file:
        file.write(f"{G_prime.number_of_edges()}\n")
        for u, v, data in G_prime.edges(data=True):
            if u not in node_mapping:
                node_mapping[u] = next_node_id
                next_node_id += 1
            if v not in node_mapping:
                node_mapping[v] = next_node_id
                next_node_id += 1
            u = node_mapping[u]
            v = node_mapping[v]
            file.write(f"{u} {v} {data['weight']}\n")
    print("Graph saved to constant_degree_graph.txt")

    return G_prime

if __name__ == "__main__":
    num_nodes = 6
    edge_probability = 0.5
    G = generate_random_graph(num_nodes, edge_probability)
    G_prime = construct_constant_degree_graph(G)