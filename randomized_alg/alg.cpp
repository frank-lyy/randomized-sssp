#include "construction.hpp"
#include "bundle_dijkstras.hpp"
#include "../graph.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

int main() {
    std::vector<std::vector<std::pair<int, double>>> graph = read_graph();
    int n = graph.size();
    std::cout << "graph size: " << n << std::endl;
    double k = std::max(sqrt(std::log(n)/std::log(std::log(n))), static_cast<double>(n)); //if n is too small, this could give a negative?
    std::cout << "k: " << k << std::endl;
    std::tuple<std::unordered_set<int>,std::vector<std::pair<double,int>>,std::unordered_map<int,std::vector<std::pair<double, int>>>,std::unordered_map<int,std::vector<std::pair<double, int>>>> construction = BundleConstruction(graph,0,k);
    std::unordered_set<int> R = std::get<0>(construction);
    std::vector<std::pair<double,int>> bundle_parents = std::get<1>(construction);
    std::unordered_map<int,std::vector<std::pair<double, int>>> bundle_map = std::get<2>(construction);
    std::unordered_map<int,std::vector<std::pair<double, int>>> ball_map = std::get<3>(construction);
    std::vector<double> distances = BundleDijkstras(R,bundle_parents,bundle_map,ball_map,graph,0,k);

    // std::vector<double> distances = std::apply(
    //     [&graph,&k](auto&&... unpackedArgs) { return BundleDijkstras(unpackedArgs..., graph, 0, k); }, //everything passed into dijkstras is empty rn for some reason?
    //     construction
    // );
    for (size_t i=0; i< n; i++) {
        std::cout << "Node: " << i << ", Distance: " << distances[i] << std::endl;
    }

    return 0;    
}