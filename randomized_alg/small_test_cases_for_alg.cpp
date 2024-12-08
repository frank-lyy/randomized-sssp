#include <iostream>
#include <fstream>
#include <vector>
#include "construction.hpp"
#include "bundle_dijkstras.hpp"
#include "globals.hpp"

long long randomized_comparison_counter = 0;
long long randomized_arithmetic_op_counter = 0;

long long dijkstras_comparison_counter = 0;
long long dijkstras_arithmetic_op_counter = 0;

int main() {
    // std::vector<std::vector<std::pair<int, double>>> graph  = {
    //     // Node 0
    //     {{1, 2.5}, {2, 3.0}},
    //     // Node 1
    //     {{0, 2.5}, {3, 1.8}, {4, 2.1}},
    //     // Node 2
    //     {{0, 3.0}, {5, 2.9}},
    //     // Node 3
    //     {{1, 1.8}, {6, 3.7}},
    //     // Node 4
    //     {{1, 2.1}, {7, 1.4}},
    //     // Node 5
    //     {{2, 2.9}, {8, 2.6}},
    //     // Node 6
    //     {{3, 3.7}, {9, 1.2}},
    //     // Node 7
    //     {{4, 1.4}, {8, 3.5}},
    //     // Node 8
    //     {{5, 2.6}, {7, 3.5}, {9, 1.9}},
    //     // Node 9
    //     {{6, 1.2}, {8, 1.9}}
    // };

    int numNodes = 1000;
    int factor_on_k = 1;
    

    std::vector<std::vector<std::pair<int, double>>> graph(numNodes);
    // Step 1: Generate a spanning tree (ensure connectivity)
    for (int i = 1; i < numNodes; ++i) {
        int parent = rand() % i; // Randomly connect to a previous node
        double weight = (rand() % 100 + 1) / 10.0; // Random weight (0.1 to 10.0)
        graph[i].emplace_back(parent, weight);
        graph[parent].emplace_back(i, weight);
    }


    std::vector<std::pair<double, int>> output = DijkstraAlgo(graph,0);
    std::ofstream outFile("../graph_generation/reference.txt");
    // Write to the file
    for (const auto& pair : output) {
        outFile << pair.second << " " << pair.first << std::endl;  
    }
    // Close the file
    outFile.close();
    // Confirm the operation
    std::cout << "Reference Dijkstra's written to reference.txt" << std::endl;

    std::cout << "Number of comparisons for Dijkstra's: " << dijkstras_comparison_counter << std::endl;
    std::cout << "Number of arithmetic ops for Dijkstra's: " << dijkstras_arithmetic_op_counter << std::endl;

    int n = numNodes;
    std::cout << "graph size: " << n << std::endl;
    // double k = std::max(sqrt(std::log(n)/std::log(std::log(n))), static_cast<double>(n)); //if n is too small, this could give a negative?
    double k = factor_on_k* sqrt(std::log(n)/std::log(std::log(n)));
    std::cout << "k: " << k << std::endl;
    const auto& [R, bundle_parents, bundle_map, ball_map] = BundleConstruction(graph,0,k);
    const std::vector<double> &distances = BundleDijkstras(R,bundle_parents,bundle_map,ball_map,graph,0,k);

    std::ofstream outFile2("../graph_generation/alg.txt");
    // Write to the file
    for (size_t i=0; i<n; i++) {
        outFile2 << i << " " << distances[i] << std::endl;  
    }
    // Close the file
    outFile2.close();
    // Confirm the operation
    std::cout << "Randomized SSSP written to output.txt" << std::endl;

    
    std::cout << "Number of comparisons for Randomized Alg: " << randomized_comparison_counter << std::endl;
    std::cout << "Number of arithmetic ops for Randomized Alg: " << randomized_arithmetic_op_counter << std::endl;
    

}
