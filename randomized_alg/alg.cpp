#include "construction.hpp"
#include "bundle_dijkstras.hpp"
#include "globals.hpp"
// #include "../graph.hpp"
#include "../constant.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int randomized_comparison_counter = 0;
int randomized_arithmetic_op_counter = 0;

int dijkstras_comparison_counter = 0;
int dijkstras_arithmetic_op_counter = 0;


int main() {
    // timing things
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;

    std::vector<std::vector<std::pair<int, double>>> graph = make_constant_degree_graph();
    int n = graph.size();
    std::cout << "graph size: " << n << std::endl;
    // double k = std::max(sqrt(std::log(n)/std::log(std::log(n))), static_cast<double>(n)); //if n is too small, this could give a negative?
    double k = sqrt(std::log(n)/std::log(std::log(n)));
    k = 5;
    std::cout << "k: " << k << std::endl;

    t1 = high_resolution_clock::now();
    const auto& [R, bundle_parents, bundle_map, ball_map] = BundleConstruction(graph,0,k);
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;
    std::cout << "Construction time: " << ms_double.count() << " ms" << std::endl;

    t1 = high_resolution_clock::now();
    const std::vector<double> &distances = BundleDijkstras(R,bundle_parents,bundle_map,ball_map,graph,0,k);
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;
    std::cout << "Dijkstras time: " << ms_double.count() << " ms" << std::endl;

    // Create an output file stream
    std::ofstream outFile("../graph_generation/alg.txt");
    // Write to the file
    for (size_t i=0; i<n; i++) {
        outFile << i << " " << distances[i] << std::endl;  
    }
    // Close the file
    outFile.close();
    // Confirm the operation
    std::cout << "Randomized SSSP written to alg.txt" << std::endl;

    
    std::cout << "Number of comparisons for Randomized Alg: " << randomized_comparison_counter << std::endl;
    std::cout << "Number of arithmetic ops for Randomized Alg: " << randomized_arithmetic_op_counter << std::endl;

    return 0;    
}