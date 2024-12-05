#include "../dijkstras.hpp"
#include "../fheap.hpp"
#include "construction.hpp"
#include <limits>

void PhaseOneRelax(int node_num, double new_dist, std::unordered_set<int>& R, FibonacciHeap<std::pair<double, int>>& H, std::vector<double> distances, std::vector<node<std::pair<double, int>>*>& pointers) {
    if (new_dist < distances[node_num]) {
        distances[node_num] = new_dist;
        if (R.find(node_num) != R.end()) {
            //use the pointer to decrease key
            H.decreaseKey(pointers[node_num], {new_dist, node_num});
        }
    }

}
void PhaseTwoRelax(int node_num, double new_dist, std::unordered_set<int>& R, FibonacciHeap<std::pair<double, int>>& H, std::vector<double> distances, std::vector<node<std::pair<double, int>>*>& pointers, std::vector<std::pair<double,int>>& bundle_parents) {
    if (new_dist < distances[node_num]) {
        distances[node_num] = new_dist;
        if (R.find(node_num) != R.end()) {
            //use the pointer to decrease key
            H.decreaseKey(pointers[node_num], {new_dist, node_num});
        } else {
            //relax the representative node
            double bundle_parent_dist = bundle_parents[node_num].first;
            int bundle_parent_node = bundle_parents[node_num].second;
            PhaseTwoRelax(bundle_parent_node, new_dist + bundle_parent_dist, R, H, distances, pointers, bundle_parents);
        }
    }
}

std::vector<double> BundleDijkstras(std::unordered_set<int>& R, std::vector<std::pair<double,int>>& bundle_parents, std::unordered_map<int,std::vector<std::pair<double, int>>>& bundle_map,std::unordered_map<int,std::vector<std::pair<double, int>>>& ball_map, std::vector<std::vector<std::pair<int, double>>> &graph, int src, int k) {
    size_t numNodes = graph.size();
    std::vector<double> distances(numNodes, std::numeric_limits<int>::max());
    std::vector<node<std::pair<double, int>>*> pointers(numNodes);
    distances[src] = 0;

    FibonacciHeap<std::pair<double, int>> H;
    
    for (const int& node_num : R) {
        node<std::pair<double, int>>* pointer = H.insert({distances[node_num],node_num});
        pointers[node_num] = pointer;
    }

    while (!H.isEmpty()) {
        std::pair<double, int> u = H.removeMinimum();
        double u_dist = u.first;
        int u_num = u.second;
        //phase 1
        for (const std::pair<double, int>& v : bundle_map[u_num]) {
            double v_u_dist = v.first;
            double v_num = v.second;
            PhaseOneRelax(v_num, u_dist+v_u_dist, R, H, distances, pointers);

            for (const std::pair<double, int>& y : ball_map[v_num]) {
                double y_v_dist = y.first;
                double y_num = y.second;
                PhaseOneRelax(v_num,distances[y_num]+y_v_dist, R, H, distances, pointers);
            }

            //for v
            for (const std::pair<int, double>& z_1 : graph[v_num]) {
                int z_1_num = z_1.first;
                double edge_weight = z_1.second;
                PhaseOneRelax(v_num,distances[z_1_num]+edge_weight, R, H, distances, pointers);
            }
            //for each z_2 in Ball(v)
            for (const std::pair<double, int>& z_2 : ball_map[v_num]) {
                double z_2_v_dist = z_2.first;
                double z_2_num = z_2.second;
                for (const std::pair<int, double>& z_1 : graph[z_2_num]) {
                    int z_1_num = z_1.first;
                    double edge_weight = z_1.second;
                    PhaseOneRelax(v_num,distances[z_1_num]+edge_weight+z_2_v_dist, R, H, distances, pointers);
                }
            }
        }

        //phase 2
        //TODO: Once we've figured out correctness and run some tests, can probably optimize this so it skips nodes we've already seen
        for (const std::pair<double, int>& x : bundle_map[u_num]) {
            double x_u_dist = x.first;
            int x_num = x.second;
            for (const std::pair<int, double>& y : graph[x_num]) {
                int y_num = y.first;
                double edge_weight = y.second;
                PhaseTwoRelax(y_num, distances[x_num]+edge_weight, R, H, distances, pointers, bundle_parents);
                for(const std::pair<double, int>& z_1 : ball_map[y_num]) {
                    double z_1_y_dist = z_1.first;
                    int z_1_num = z_1.second;
                    PhaseTwoRelax(z_1_num, distances[x_num]+edge_weight+z_1_y_dist, R, H, distances, pointers, bundle_parents);
                }
            }
        }
    }

    return distances;
}