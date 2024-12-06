import json

def evaluate_correctness():
    reference_filename = "reference.txt"
    alg_filename = "alg.txt"
    mappings_filename = "node_mappings.json"

    with open(mappings_filename) as f:
        mappings = json.load(f)

    reference_distances = {}
    with open(reference_filename) as f:
        for line in f:
            node, distance = line.split()
            reference_distances[node] = distance
    print(reference_distances)
    
    with open(alg_filename) as f:
        for line in f:
            node, distance = line.split()
            if reference_distances[mappings[node]] != distance:
                print(f"Distance was wrong for node {mappings[node]}")
                print(f"Expected: {reference_distances[mappings[node]]}, Actual: {distance}")
                return False
    
    print("All nodes have the same distance as reference")
    return True

if __name__ == "__main__":
    evaluate_correctness()
