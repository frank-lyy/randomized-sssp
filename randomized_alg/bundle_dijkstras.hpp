#include "../fheap.hpp"
#include "globals.hpp"
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <limits>
#include <iostream>
#include <tuple>
#include <memory> 

inline void PhaseOneRelax(
    int node_num, double new_dist, 
    std::unordered_set<int>& R, 
    FibonacciHeap<std::pair<double, int>>& H, 
    std::vector<double>& distances, 
    std::vector<node<std::pair<double, int>>*>& pointers) 
{
    randomized_comparison_counter++;
    if (new_dist < distances[node_num]) {
        distances[node_num] = new_dist;
        randomized_comparison_counter++;
        if (R.find(node_num) != R.end()) {
            //use the pointer to decrease key
            H.decreaseKey(pointers[node_num], {new_dist, node_num});
        }
    }

}
inline void PhaseTwoRelax(
    int node_num, double new_dist, 
    std::unordered_set<int>& R, 
    FibonacciHeap<std::pair<double, int>>& H, 
    std::vector<double>& distances, 
    std::vector<node<std::pair<double, int>>*>& pointers, 
    std::vector<std::pair<double,int>>& bundle_parents) 
{
    randomized_comparison_counter++;
    if (new_dist < distances[node_num]) {
        distances[node_num] = new_dist;
        randomized_comparison_counter++;
        if (R.find(node_num) != R.end()) {
            //use the pointer to decrease key
            H.decreaseKey(pointers[node_num], {new_dist, node_num});
        } else {
            //relax the representative node
            double bundle_parent_dist = bundle_parents[node_num].first;
            int bundle_parent_node = bundle_parents[node_num].second;
            randomized_arithmetic_op_counter++;
            PhaseTwoRelax(bundle_parent_node, new_dist + bundle_parent_dist, R, H, distances, pointers, bundle_parents);
        }
    }
}

std::vector<double> BundleDijkstras(
    const std::unique_ptr<std::unordered_set<int>>& R_ptr, 
    const std::unique_ptr<std::vector<std::pair<double,int>>>& bundle_parents_ptr, 
    const std::unique_ptr<std::unordered_map<int,std::vector<std::pair<double, int>>>>& bundle_map_ptr, 
    const std::unique_ptr<std::unordered_map<int,std::vector<std::pair<double, int>>>>& ball_map_ptr, 
    std::vector<std::vector<std::pair<int, double>>> &graph, int src, int k) 
{
    auto& R = *R_ptr;
    auto& bundle_parents = *bundle_parents_ptr;
    auto& bundle_map = *bundle_map_ptr;
    auto& ball_map = *ball_map_ptr;

    size_t numNodes = graph.size();
    std::vector<double> distances(numNodes, std::numeric_limits<int>::max());
    std::vector<node<std::pair<double, int>>*> pointers(numNodes);
    // std::unordered_set<int> popped_nodes;
    // std::vector<std::pair<double, int>> output;
    // std::unordered_map<int, node<std::pair<double, int>>*> pointers;
    distances[src] = 0;

    FibonacciHeap<std::pair<double, int>> H;
    
    for (const int& node_num : R) {
        node<std::pair<double, int>>* pointer = H.insert({distances[node_num],node_num});
        pointers[node_num] = pointer;
    }

    randomized_comparison_counter++;
    while (!H.isEmpty()) {
        auto start_node = H.removeMinimum();
        auto [u_dist, u_num] = start_node;
        // pointers.erase(start_node.second);
        // popped_nodes.insert(start_node.second);
        // output.push_back(start_node);
        //phase 1
        for (const auto& [v_u_dist,v_num] : bundle_map[u_num]) {
            // if (popped_nodes.find(v_num) != popped_nodes.end()) {
            //     continue;
            // }
            randomized_arithmetic_op_counter++;
            PhaseOneRelax(v_num, u_dist+v_u_dist, R, H, distances, pointers);

            for (const auto& [y_v_dist,y_num] : ball_map[v_num]) {
                // if (popped_nodes.find(y_num) != popped_nodes.end()) {
                //     continue;
                // }
                randomized_arithmetic_op_counter++;
                PhaseOneRelax(v_num,distances[y_num]+y_v_dist, R, H, distances, pointers);
            }

            //for v
            for (const auto& [z_1_num, edge_weight] : graph[v_num]) {
                randomized_arithmetic_op_counter++;
                PhaseOneRelax(v_num,distances[z_1_num]+edge_weight, R, H, distances, pointers);
            }
            //for each z_2 in Ball(v)
            for (const auto& [z_2_v_dist,z_2_num] : ball_map[v_num]) {
                for (const auto& [z_1_num, edge_weight] : graph[z_2_num]) {
                    randomized_arithmetic_op_counter++;
                    PhaseOneRelax(v_num,distances[z_1_num]+edge_weight+z_2_v_dist, R, H, distances, pointers);
                }
            }
        }

        //phase 2
        //TODO: Once we've figured out correctness and run some tests, can probably optimize this so it skips nodes we've already seen
        for (const auto& [x_u_dist,x_num] : bundle_map[u_num]) {
            for (const auto& [y_num, edge_weight] : graph[x_num]) {
                randomized_arithmetic_op_counter++;
                PhaseTwoRelax(y_num, distances[x_num]+edge_weight, R, H, distances, pointers, bundle_parents);
                for(const auto& [z_1_y_dist, z_1_num] : ball_map[y_num]) {
                    randomized_arithmetic_op_counter++;
                    PhaseTwoRelax(z_1_num, distances[x_num]+edge_weight+z_1_y_dist, R, H, distances, pointers, bundle_parents);
                }
            }
        }

        randomized_comparison_counter++;
    }

    return distances;
}