#include <vector>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include "../dijkstras.hpp"
#include "globals.hpp"
#include <random>
#include <cmath>
#include <iostream>
#include <tuple>
#include <memory> 
using namespace std;

bool ChanceBool(double probabilityTrue) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(probabilityTrue);

    return dis(gen);
}

//TODO: Change return type, should give data structures for Bundles and Balls
std::tuple<
    std::unique_ptr<std::unordered_set<int>>,
    std::unique_ptr<std::vector<std::pair<double, int>>>,
    std::unique_ptr<std::unordered_map<int, std::vector<std::pair<double, int>>>>,
    std::unique_ptr<std::unordered_map<int, std::vector<std::pair<double, int>>>>
> BundleConstruction(std::vector<std::vector<std::pair<int, double>>> &graph, int src, double k) 
{
    std::unordered_set<int> R {src}; //initialize R_1 with the source node in it
    size_t numNodes = graph.size();
    std::cout << "k (inside construction): " << k << "\n";
    randomized_arithmetic_op_counter++;
    double chance = 1.0 / k;
    std::cout << "Chance: " << chance << "\n";
    randomized_comparison_counter++;
    for (size_t i=0; i<numNodes; i++) {
        randomized_comparison_counter++;
        if(ChanceBool(chance)) {
            // std::cout << "Randomly selected node: " << i << "\n";
            R.insert(i);
        }
        randomized_arithmetic_op_counter++;
        randomized_comparison_counter++;
    }
    std::cout << "Randomly selected " << R.size() << " nodes\n";

    randomized_arithmetic_op_counter++;
    double node_limit = k * std::log(k); //compute klogk with natural log.

    std::unordered_map<int,std::vector<std::pair<double, int>>> V_extract_map;     

    //Run dijkstras from every v in V \ R_1, and add in the R_2 nodes to R_1 as you go.
    randomized_comparison_counter++;
    for (size_t i=0; i< numNodes; i++) {
        randomized_comparison_counter++;
        if (R.find(i) == R.end()) { //if node i is not in r, run dijkstras
            std::vector<std::pair<double, int>> V_extract = DijkstraAlgoLazy(graph, i, R, node_limit);
            randomized_comparison_counter++;
            if(V_extract.empty()) {
                R.insert(i);
            } else {
                V_extract_map.insert({i, V_extract});
            }
        }
        randomized_arithmetic_op_counter++;
        randomized_comparison_counter++;
    }

    // DEBUG OUTPUT FOR V_extract_map
    // for (const auto& [key, valueVector] : V_extract_map) {
    //     std::cout << "Node " << key << ":\n";

    //     // Inner loop: iterate through the vector of pairs
    //     for (const auto& [distance, destination] : valueVector) {
    //         std::cout << "  destination: " << destination << ", Distance: " << distance << "\n";
    //     }
    // }


    std::vector<std::pair<double,int>> bundle_parents(numNodes);
    std::unordered_map<int,std::vector<std::pair<double, int>>> bundle_map;
    for (const int& key : R) {
        bundle_map[key] = {{0,key}}; // Initialize the bundle with itself
    }

    //walk through each V_extract until you find the first node in R
    //build the bundles as we do this
    for (auto V_extract_pair: V_extract_map) {
        int start_node = V_extract_pair.first;
        std::vector<std::pair<double, int>> V_extract = V_extract_pair.second;
        randomized_comparison_counter++;
        for (size_t i=0; i<V_extract.size(); i++) {
            std::pair<double, int> node = V_extract[i];
            int node_num = node.second;
            randomized_comparison_counter++;
            if (R.find(node_num) != R.end()) {
                //delete all nodes after this one from V_extract
                randomized_comparison_counter++;
                randomized_arithmetic_op_counter++;
                if (i + 1 < V_extract.size()) {
                    randomized_arithmetic_op_counter++;
                    V_extract.erase(V_extract.begin() + i + 1, V_extract.end());
                }
                //add to bundle_map
                bundle_map[node_num].push_back({node.first,start_node});
                //add to bundle_parents
                bundle_parents[start_node] = node;
                break;
            }
            randomized_arithmetic_op_counter++;
            randomized_comparison_counter++;

        }
    }

    // DEBUG OUTPUT FOR bundle_map
    // for (const auto& [key, valueVector] : bundle_map) {
    //     std::cout << "Parent: " << key << ":\n";

    //     // Inner loop: iterate through the vector of pairs
    //     for (const auto& [distance, destination] : valueVector) {
    //         std::cout << "  child: " << destination << ", Distance: " << distance << "\n";
    //     }
    // }

    // DEBUG OUTPUT FOR bundle_parents
    // for (size_t i = 0; i< bundle_parents.size(); i++) {
    //     const auto& [distance, parent] =bundle_parents[i];
    //     std::cout << "  child: " << parent << ", Distance: " << distance << "\n";
    // }

    //build the balls
    std::unordered_map<int,std::vector<std::pair<double, int>>> ball_map = V_extract_map;
    for (auto &item: ball_map) {
        auto& ball = item.second;
        randomized_arithmetic_op_counter++;
        double dist_to_R = ball[ball.size()-1].first;
        bool found = false;
        randomized_arithmetic_op_counter++;
        randomized_comparison_counter++;
        for (int i=ball.size()-1; i>=0; i--) {
            randomized_comparison_counter++;
            double distance = ball[i].first;
            if (distance < dist_to_R) {
                found = true;
                randomized_arithmetic_op_counter++;
                ball.erase(ball.begin() + i + 1, ball.end());
                break;
            }
            randomized_arithmetic_op_counter++;
            randomized_comparison_counter++;
        }
        randomized_comparison_counter++;
        if(!found) {
            ball.clear(); //TODO: Make a test case for this where every node in the extract map has distance 0, so the ball should be empty
        }
    }

    //DEBUG OUTPUT FOR ball_map
    // for (const auto& [key, valueVector] : ball_map) {
    //     std::cout << "Node " << key << ":\n";
    //     // Inner loop: iterate through the vector of pairs
    //     for (const auto& [distance, destination] : valueVector) {
    //         std::cout << "  destination: " << destination << ", Distance: " << distance << "\n";
    //     }
    // }
    auto R_ptr = std::make_unique<std::unordered_set<int>>(std::move(R));
    auto bundle_parents_ptr = std::make_unique<std::vector<std::pair<double,int>>>(std::move(bundle_parents));
    auto bundle_map_ptr = std::make_unique<std::unordered_map<int,std::vector<std::pair<double, int>>>>(std::move(bundle_map));
    auto ball_map_ptr = std::make_unique<std::unordered_map<int,std::vector<std::pair<double, int>>>>(std::move(ball_map));
    return std::make_tuple(std::move(R_ptr), std::move(bundle_parents_ptr), std::move(bundle_map_ptr), std::move(ball_map_ptr));


    
}