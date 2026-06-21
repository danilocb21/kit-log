#include <algorithm>
#include <random>
#include <unordered_set>
#include <vector>

#include "construction.h"

double construction::construc(double** costs, int n) {
    // std::random_device rd;
    // std::mt19937 rng(rd());
    // std::uniform_real_distribution<> rd_double(0.0, 0.25);

    // double alpha = rd_double(rng);

    double cost = 0.0;
    int last_node = 0;
    
    std::unordered_set<int> CL;
    for (int i = 1; i < n; i++)
        CL.insert(i);
    
    while (!CL.empty()) {
        std::vector<InsertionInfo> insertion_costs;
        insertion_costs.reserve((CL.size()));

        int r = last_node;

        for (int node : CL) {
            double cost = costs[r][node];
            insertion_costs.emplace_back(node, cost);
        }

        sort(insertion_costs.begin(), insertion_costs.end());

        // std::uniform_int_distribution<> dist(0, alpha * static_cast<double>(insertion_costs.size() - 1));

        // int selected = dist(rng);

        int selected = 0; // first node
        
        auto &x = insertion_costs[selected];
        CL.erase(x.inserted_node);
        last_node = x.inserted_node;
        cost += x.cost;
    }
    cost += costs[last_node][0];

    return cost;
}