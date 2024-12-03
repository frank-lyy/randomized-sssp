#include <vector>
#include <limits>
#include <unordered_set>
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

//Dijkstra with a set of nodes R and a limit of nodes it can pop from the queue. Once it reaches this limit, it aborts and returns an empty vector.
std::vector<std::pair<double, int>> DijkstraAlgoLazy(std::vector<std::vector<std::pair<int, double>>> &graph, int src, std::unordered_set<int> R, int node_limit) {
    int popped_counter = 0;
    
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
        
        //check if the node is in the set R. If so, we have completed this Dijkstra's and we can return
        if (R.find(start_node.second) != R.end()) {
            return output;
        }

        //increment the counter, if we reach our limit return empty vector
        popped_counter++;
        if (popped_counter >= node_limit) {
            return {};
        }

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