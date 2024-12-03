#include <vector>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include "../dijkstras.hpp"
#include <random>
#include <cmath>
using namespace std;

bool ChanceBool(double probabilityTrue) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dis(probabilityTrue);

    return dis(gen);
}

//TODO: Change return type, should give data structures for Bundles and Balls
std::vector<std::pair<double, int>> BundleConstruction(std::vector<std::vector<std::pair<int, double>>> &graph, int src, int k) 
{
    std::unordered_set<int> R {src}; //initialize R_1 with the source node in it
    size_t numNodes = graph.size();
    double chance = 1/k;
    for (size_t i=0; i<numNodes; i++) {
        if(ChanceBool(chance)) {
            R.insert(i);
        }
    }

    double node_limit = k * std::log(k); //compute klogk with natural log.

    std::unordered_map<int,std::vector<std::pair<double, int>>> V_extract_map;     

    //Run dijkstras from every v in V \ R_1, and add in the R_2 nodes to R_1 as you go.
    for (size_t i=0; i< numNodes; i++) {
        if (R.find(i) == R.end()) { //if node i is not in r, run dijkstras
            std::vector<std::pair<double, int>> V_extract = DijkstraAlgoLazy(graph, src, R, node_limit);
            if(V_extract.empty()) {
                R.insert(i);
            } else {
                V_extract_map.insert({i, V_extract});
            }
        }
    }

    //walk through each V_extract until you find the first node in R
    for (auto i: V_extract_map) {
        int start_node = i.first;
        std::vector<std::pair<double, int>> V_extract = i.second;
        for (size_t i=0; i<V_extract.size(); i++) {
            std::pair<double, int> node = V_extract[i];
            int node_num = node.second;
            if (R.find(node_num) != R.end()) {
                //delete all nodes after this one from V_extract
                if (i + 1 < V_extract.size()) {
                    V_extract.erase(V_extract.begin() + i + 1, V_extract.end());
                }
                break;
            }

        }
    }

    //build bundles and balls
}