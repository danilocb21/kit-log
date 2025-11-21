#include <vector>
#include <unordered_set>
#include <algorithm>

#include "MLP.hpp"
#include "solution.hpp"

struct InsertionInfo {
    int inserted_node;
    double cost;

    bool operator<(const InsertionInfo &p) const {
        return cost < p.cost;
    }

    InsertionInfo(int inserted_node, double cost) : inserted_node(inserted_node), cost(cost) {}
};

Solution MLP::rand_cheapest_insertion(const double alpha) {
    Solution solution{};
    solution.sequence.push_back(1);

    int n = m_instance.getDimension();

    unordered_set<int> CL;
    for (int i = 2; i <= static_cast<int>(n); i++)
        CL.insert(i);
    
    while (!CL.empty()) {
        vector<InsertionInfo> insertion_costs;
        insertion_costs.reserve((CL.size()));

        int r = solution.sequence.back();

        for (int node : CL) {
            double cost = dist_matrix[r][node];
            insertion_costs.emplace_back(node, cost);
        }

        sort(insertion_costs.begin(), insertion_costs.end());

        std::uniform_int_distribution<> dist(0, static_cast<size_t>(alpha * static_cast<double>(insertion_costs.size() - 1)));
        size_t selected = dist(m_generator);
        
        auto &x = insertion_costs[selected];
        CL.erase(x.inserted_node);
        solution.sequence.push_back(x.inserted_node);
        solution.cost += x.cost;
    }

    solution.cost += dist_matrix[solution.sequence.back()][1];
    solution.sequence.push_back(1);

    return solution;
}