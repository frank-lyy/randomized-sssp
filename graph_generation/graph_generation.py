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
    Constructs G' from G, transforming it into a constant-degree graph while keeping nodes labeled as integers.
    Node IDs for G' will be continuously increasing integers.
    :param G: A weighted undirected graph (NetworkX Graph or DiGraph)
    :return: The transformed graph G' and a mapping from (original_node, neighbor) to new_node_id
    """
    G_prime = nx.Graph()  # G' will be an undirected graph
    next_node_id = max(G.nodes()) + 1  # Start assigning new node IDs after the largest ID in G
    
    node_map = {}  # Map (v, w) -> new integer node ID for cycle nodes in G'

    # Step 1: Replace each vertex v with a cycle of |N(v)| vertices
    for v in G.nodes():
        neighbors = list(G.neighbors(v))
        
        # Create a new node for each neighbor in the cycle
        cycle_nodes = []
        for w in neighbors:
            node_map[(v, w)] = next_node_id
            cycle_nodes.append(next_node_id)
            next_node_id += 1

        # Add zero-weight edges to form the cycle
        for i in range(len(cycle_nodes)):
            G_prime.add_edge(cycle_nodes[i], cycle_nodes[(i + 1) % len(cycle_nodes)], weight=0)

    # Step 2: Add edges for every edge (u, v) in G
    for u, v, data in G.edges(data=True):
        weight = data.get("weight", 1)  # Default weight is 1 if not provided
        
        # Add edge between the corresponding cycle nodes in G'
        G_prime.add_edge(node_map[(u, v)], node_map[(v, u)], weight=weight)

    return G_prime, node_map

if __name__ == "__main__":
    num_nodes = 6
    edge_probability = 0.5
    G = generate_random_graph(num_nodes, edge_probability)
    G_prime = construct_constant_degree_graph(G)