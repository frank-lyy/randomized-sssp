#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

inline std::vector<std::vector<std::pair<int, double>>> make_constant_degree_graph() {
    std::string filename;
    filename = "../graph_generation/graph.txt";
    std::ifstream inputFile(filename);
    // if (!inputFile.is_open()) {
    //     std::cerr << "Error: Could not open file.\n";
    //     return {{{1,1}}};
    // }

    int numNodes;
    inputFile >> numNodes;
    // Adjacency list representation
    std::vector<std::vector<std::pair<int, double>>> g(numNodes, std::vector<std::pair<int, double>>());

    // Read edges from file
    int newEdges = 0;
    int u, v;
    double w;
    while (inputFile >> u >> v >> w) {
        g[u].push_back(std::make_pair(v, w));
        newEdges += 2;
        // g[v].push_back(std::make_pair(u, w)); // For undirected graph
    }
    inputFile.close();

    std::vector<std::vector<std::pair<int, double>>> g_prime(newEdges, std::vector<std::pair<int, double>>());
    std::vector<std::vector<int>> nodeMappings(numNodes);
    int nextNodeId = 0;

    std::ofstream graphFile("../graph_generation/constant_degree_graph.txt");
    graphFile << newEdges << std::endl;
    std::ofstream mappingFile("../graph_generation/node_mappings.txt");
    // if (!graphFile || !mappingFile) {
    //     std::cerr << "Error opening output files!" << std::endl;
    //     return;
    // }

    for (int u = 0; u < g.size(); ++u) {
        const auto& neighbors = g[u];

        for (const auto& edge : neighbors) {
            int v = edge.first;
            double w = edge.second;

            int newU = nextNodeId;
            int newV = nextNodeId + 1;
            nextNodeId += 2;

            nodeMappings[u].push_back(newU);
            nodeMappings[v].push_back(newV);

            graphFile << newU << " " << newV << " " << w << "\n";
            g_prime[newU].push_back(std::make_pair(newV, w));
            g_prime[newV].push_back(std::make_pair(newU, w));
        }
    }

    // Add edges for each edge in the original graph
    for (int u = 0; u < g.size(); ++u) {
        std::vector<int> cycleNodes = nodeMappings[u];
        int cycleSize = cycleNodes.size();
        if (cycleSize > 1) {
            for (int i = 0; i < cycleSize; ++i) {
                int from = cycleNodes[i];
                int to = cycleNodes[(i + 1) % cycleSize];
                g_prime[from].push_back(std::make_pair(to, 0.0));
                g_prime[to].push_back(std::make_pair(from, 0.0));
                graphFile << from << " " << to << " 0.0\n";
                mappingFile << nodeMappings[u][i] << " " << u << "\n";
            }
        } else {
            mappingFile << nodeMappings[u][0] << " " << u << "\n";
        }
    }

    graphFile.close();
    mappingFile.close();
    std::cout << "Graph saved to constant_degree_graph.txt and node mappings to node_mappings.txt" << std::endl;
    return g_prime;
}