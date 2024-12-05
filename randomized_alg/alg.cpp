#include "construction.hpp"
#include "bundle_dijkstras.hpp"
#include "../graph.hpp"
#include <iostream>
#include <vector>
#include <cmath>

int main() {
    std::vector<std::vector<std::pair<int, double>>> graph = read_graph();
    int n = graph.size();
    double k = sqrt(std::log(n)/std::log(std::log(n)));
    std::tuple<std::unordered_set<int>,std::vector<std::pair<double,int>>,std::unordered_map<int,std::vector<std::pair<double, int>>>,std::unordered_map<int,std::vector<std::pair<double, int>>>> construction = BundleConstruction(graph,0,k);
    std::vector<double> distances = std::apply(
        [&graph,&k](auto&&... unpackedArgs) { return BundleDijkstras(unpackedArgs..., graph, 0, k); },
        construction
    );
    for (size_t i=0; i< n; i++) {
        std::cout << "Node: " << i << ", Distance: " << distances[i] << std::endl;
    }

    return 0;    
}