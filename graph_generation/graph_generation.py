import networkx as nx
import json
import random

def generate_random_graph(num_nodes, edge_probability):
    # Generate a small random graph
    G = nx.erdos_renyi_graph(n=num_nodes, p=edge_probability, directed=False)  
    while not nx.is_connected(G):
        print("Graph is not connected. Generating a new graph...")
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

def generate_random_graph_manual(num_nodes, num_extra_edges):
    # Initialize the graph as a list of adjacency lists
    G = [[] for _ in range(num_nodes)]

    # Step 1: Generate a spanning tree (ensure connectivity)
    edges = set()
    for i in range(1, num_nodes):
        parent = random.randint(0, i - 1)  # Randomly connect to a previous node
        weight = round(random.uniform(1.0, 10.0),2)  # Random weight (1.0 to 10.0)
        edges.add((i, parent))
        edges.add((parent, i))
        G[i].append((parent, weight))
        G[parent].append((i, weight))  # Undirected edge

    # Step 2: Add extra random edges for more complexity
    for _ in range(num_extra_edges):
        u = random.randint(0, num_nodes - 1)
        v = random.randint(0, num_nodes - 1)
        while u == v or (u,v) in edges:  # Avoid self-loops
            v = random.randint(0, num_nodes - 1)

        weight = round(random.uniform(1.0, 10.0),2)  # Random weight (1.0 to 10.0)
        G[u].append((v, weight))
        G[v].append((u, weight))  # Undirected edge
        edges.add((i, parent))
        edges.add((parent, i))

    with open("graph.txt", "w") as file:
        file.write(f"{num_nodes}\n")
        for u, adjacency in enumerate(G):
            for (v, weight) in adjacency:
                if u<v:
                    file.write(f"{u} {v} {weight}\n")
    print("Graph saved to graph.txt")

    return G

def construct_constant_degree_graph(G):
    """
    Constructs G' from G such that each vertex in G' has degree at most 3.
    :param G: A weighted undirected graph (NetworkX Graph or DiGraph)
    :return: The transformed graph G'
    """
    G_prime = nx.Graph()  # G' will be an undirected graph

    g_node_mappings = {}
    # Iterate through all nodes in G
    for v in G.nodes():
        neighbors = list(G.neighbors(v))
        
        # Create a cycle of |N(v)| vertices for v
        cycle_nodes = [f"{v}_{w}" for w in neighbors]  # Unique labels for the cycle nodes
        if len(neighbors) == 0: 
            cycle_nodes.append(f"{v}_{v}")
        
        # Add mappings from cycle nodes to v
        for c in cycle_nodes:
            g_node_mappings[c] = v
        
        if len(cycle_nodes) == 1:
            G_prime.add_node(cycle_nodes[0])
            continue # Skip if v has only one neighbor
        for i in range(len(neighbors)):
            G_prime.add_edge(cycle_nodes[i], cycle_nodes[(i + 1) % len(neighbors)], weight=0)  # Zero-weight cycle
    # print(g_node_mappings)

    # Add edges for each edge in G
    for u, v, data in G.edges(data=True):
        # weight = data.get("weight", 1)  # Default weight is 1 if not provided
        G_prime.add_edge(f"{u}_{v}", f"{v}_{u}", weight=data["weight"])  # Connect corresponding cycle nodes
    
    next_node_id = 0
    node_mapping = {}
    # Save the graph to a file in edge list format
    with open("constant_degree_graph.txt", "w") as file:
        file.write(f"{G_prime.number_of_nodes()}\n")
        for u in G_prime.nodes():
            if u not in node_mapping:
                node_mapping[u] = next_node_id
                next_node_id += 1
        for u, v, data in G_prime.edges(data=True):
            u = node_mapping[u]
            v = node_mapping[v]
            file.write(f"{u} {v} {data['weight']}\n")
    print("Graph saved to constant_degree_graph.txt")
    # print(node_mapping)

    g_prime_node_mappings = {str(node_in_g_prime): str(g_node_mappings[node_in_g_prime_str]) for node_in_g_prime_str, node_in_g_prime in node_mapping.items()}
    with open("node_mappings.json", "w") as file:
        json.dump(g_prime_node_mappings, file)
    print("Mappings saved to node_mappings.json")

    return G_prime

if __name__ == "__main__":
    num_nodes = 10000000
    num_extra_edges = 10000
    G = generate_random_graph_manual(num_nodes, num_extra_edges)
    # dists = nx.shortest_path_length(G, source=0, weight="weight")
    # with open("true_reference.txt", "w") as file:
    #     for node, dist in dists.items():
    #         file.write(f"{node} {dist}\n")
    # G_prime = construct_constant_degree_graph(G)