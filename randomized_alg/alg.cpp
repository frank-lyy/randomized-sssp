#include "construction.hpp"
#include "bundle_dijkstras.hpp"
#include "globals.hpp"
#include "../graph.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

int randomized_comparison_counter = 0;
int randomized_arithmetic_op_counter = 0;

int dijkstras_comparison_counter = 0;
int dijkstras_arithmetic_op_counter = 0;


int main() {
    std::vector<std::vector<std::pair<int, double>>> graph = read_graph(true);
    int n = graph.size();
    std::cout << "graph size: " << n << std::endl;
    // double k = std::max(sqrt(std::log(n)/std::log(std::log(n))), static_cast<double>(n)); //if n is too small, this could give a negative?
    double k = sqrt(std::log(n)/std::log(std::log(n)));
    std::cout << "k: " << k << std::endl;
    const auto& [R, bundle_parents, bundle_map, ball_map] = BundleConstruction(graph,0,k);
    const std::vector<double> &distances = BundleDijkstras(R,bundle_parents,bundle_map,ball_map,graph,0,k);

    // std::vector<double> distances = std::apply(
    //     [&graph,&k](auto&&... unpackedArgs) { return BundleDijkstras(unpackedArgs..., graph, 0, k); }, //everything passed into dijkstras is empty rn for some reason?
    //     construction
    // );
    // for (size_t i=0; i< n; i++) {
    //     std::cout << "Node: " << i << ", Distance: " << distances[i] << std::endl;
    // }

    // Create an output file stream
    std::ofstream outFile("../graph_generation/alg.txt");
    // Write to the file
    for (size_t i=0; i<n; i++) {
        outFile << i << " " << distances[i] << std::endl;  
    }
    // Close the file
    outFile.close();
    // Confirm the operation
    std::cout << "Randomized SSSP written to output.txt" << std::endl;

    
    std::cout << "Number of comparisons for Randomized Alg: " << randomized_comparison_counter << std::endl;
    std::cout << "Number of arithmetic ops for Randomized Alg: " << randomized_arithmetic_op_counter << std::endl;

    return 0;    
}