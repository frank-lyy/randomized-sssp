#include <iostream>
#include <vector>
#include "dijkstras.hpp"
#include "graph.hpp"
#include <unordered_set>

int main() {
    std::vector<std::vector<std::pair<int, double>>> graph = read_graph();
    std::unordered_set<int> R {0};
    std::vector<std::pair<double, int>> output = DijkstraAlgoLazy(graph,4,R,10);
    std::cout << "Vector contents:" << std::endl;
    for (const auto& pair : output) {
        std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
    }

    return 0;    
}