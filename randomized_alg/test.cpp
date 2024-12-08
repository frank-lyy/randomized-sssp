#include <iostream>
#include <vector>
#include "../dijkstras.hpp"
#include "../graph.hpp"
#include <unordered_set>
#include "globals.hpp"

int randomized_comparison_counter = 0;
int randomized_arithmetic_op_counter = 0;

int dijkstras_comparison_counter = 0;
int dijkstras_arithmetic_op_counter = 0;

int main() {
    std::vector<std::vector<std::pair<int, double>>> graph = read_graph();
    std::vector<std::pair<double, int>> output = DijkstraAlgo(graph,0);
    // std::cout << "Vector contents:" << std::endl;
    // for (const auto& pair : output) {
    //     std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
    // }

    // Create an output file stream
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

    return 0;    
}