#include <vector>
#include <limits>
#include "fheap.hpp"
using namespace std;

// graph is an adjacency list with edge weights (list of list of pairs adj_node,weight)
std::vector<std::pair<double, int>> DijkstraAlgo(std::vector<std::vector<std::pair<int, double>>> &graph, int src) 
{
    size_t numNodes = graph.size();
    // Initialize a vector with size 'numNodes' and all values set to MAX_INT
    std::vector<int> distances(numNodes, std::numeric_limits<int>::max());
    std::vector<node<std::pair<double, int>>*> pointers(numNodes);
    distances[src] = 0;

    std::vector<std::pair<double, int>> output;            
    
    FibonacciHeap<std::pair<double, int>> H;

    for(size_t i = 0; i < distances.size(); ++i) {
        node<std::pair<double, int>>* pointer = H.insert({distances[i],i});
        pointers[i] = pointer;
    }

    while (!H.isEmpty()) {
        std::pair<double, int> start_node = H.removeMinimum();
        output.push_back(start_node); //push to our output (distance,node)
        int node_num = start_node.second;
        double node_dist = start_node.first;
        std::vector<std::pair<int, double>> edges = graph[node_num];
        for (std::pair<int, double> adj: edges) {
            int adj_node_num = adj.first;
            double edge_weight = adj.second;
            node<std::pair<double, int>> adj_node = *pointers[adj_node_num];
            int adj_node_dist = adj_node.getValue().first;

            //try to relax
            if (node_dist + edge_weight < adj_node_dist) {
                double new_adj_node_dist = node_dist + edge_weight;
                H.decreaseKey(pointers[adj_node_num], {new_adj_node_dist, adj_node_num});
            }

        }
    }

    return output;
    
    
}