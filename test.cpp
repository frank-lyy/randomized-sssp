#include <iostream>
#include <vector>
#include "dijkstras.hpp"
#include "graph.hpp"

int main() {
    std::vector<std::vector<std::pair<int, double>>> graph = read_graph();
    std::vector<std::pair<double, int>> output = DijkstraAlgo(graph,0);
    std::cout << "Vector contents:" << std::endl;
    for (const auto& pair : output) {
        std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
    }

    return 0;    
}