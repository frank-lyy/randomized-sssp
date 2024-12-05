#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

inline std::vector<std::vector<std::pair<int, double>>> read_graph() {
    std::ifstream inputFile("../graph_generation/small_graph.txt");
    // if (!inputFile.is_open()) {
    //     std::cerr << "Error: Could not open file.\n";
    //     return {{{1,1}}};
    // }

    int numNodes;
    inputFile >> numNodes;
    // Adjacency list representation
    std::vector<std::vector<std::pair<int, double>>> graph(numNodes, std::vector<std::pair<int, double>>());

    // Read edges from file
    int u, v;
    double w;
    while (inputFile >> u >> v >> w) {
        graph[u].push_back(std::make_pair(v, w));
        graph[v].push_back(std::make_pair(u, w)); // For undirected graph
    }
    inputFile.close();

    // Print adjacency list
    for (int i = 0; i < numNodes; i++) {
        std::cout << "Node " << i << ": ";
        for (auto neighbor : graph[i]) {
            std::cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        }
        std::cout << "\n";
    }

    return graph;
}
