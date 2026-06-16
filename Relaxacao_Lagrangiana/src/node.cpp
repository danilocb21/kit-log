#include <algorithm>
#include <vector>

#include "bb.h"
#include "node.h"
#include "Kruskal.h"

void Node::update(std::vector<std::vector<double>> &costs, double ub) {
    std::vector<std::pair<std::pair<int, int>, double>> prev_vals;
    for (auto arc : this->forbidden_arcs) {
        int i = arc.first;
        int j = arc.second;
        prev_vals.emplace_back(arc, costs[i][j]);
        costs[i][j] = INFINITE;
    }

    this->graph = subgradient_tsp(costs, ub);

    // this->degree = get_degrees(n);
    this->chosen = get_chosen();
    // this->feasible = is_feasible();

    for (auto arc : prev_vals) {
        int i = arc.first.first;
        int j = arc.first.second;
        double c = arc.second;
        costs[i][j] = c;
    }
}

std::vector<Edge> Node::subgradient_tsp(std::vector<std::vector<double>> &costs, double ub) {
    double eps = 1.0;
    double eps_min = 1e-5;
    
    int iter = 0;
    int iter_max = 30;
    
    double lb = 0.0;
    std::vector<Edge> best_edges;
    
    while (eps > eps_min) {
        Kruskal krsk(n);
        std::pair<double, std::vector<Edge>> mst = krsk.find_mst(costs, lambdas);
        break;
    }
    return best_edges;
}

// Mudar para retornar o nó de maior grau
int Node::get_chosen() { return 0; } // a lista de subtours já esta ordenada pelo menor tamanho

// bool Node::is_feasible() { return this->subtour.size() == 1UL; }
