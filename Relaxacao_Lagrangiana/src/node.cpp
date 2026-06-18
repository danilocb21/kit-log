#include <algorithm>
#include <vector>

#include "bb.h"
#include "node.h"
#include "Kruskal.h"

void Node::update(std::vector<std::vector<double>> &costs, double ub) {
    std::vector<Edge> prev_vals;
    for (auto arc : this->forbidden_arcs) {
        int i = arc.first;
        int j = arc.second;
        prev_vals.emplace_back(costs[i][j], arc);
        costs[i][j] = INFINITE;
    }

    subgradient_tsp(costs, ub);
    get_degrees();
    get_chosen();
    this->feasible = is_feasible();

    for (auto arc : prev_vals) {
        double c = arc.first;
        int i = arc.second.first;
        int j = arc.second.second;
        costs[i][j] = c;
    }
}

void Node::subgradient_tsp(std::vector<std::vector<double>> &costs, double ub) {
    double eps = 1.0;
    double eps_min = 1e-5;
    
    int iter = 0;
    int iter_max = 30;
    
    double best_lb = 0.0;
    std::vector<Edge> best_edges;

    std::vector<double> p_lambda = lambdas;
    
    while (eps > eps_min) {
        Kruskal krsk(n);
        std::pair<double, std::vector<Edge>> mst = krsk.find_mst(costs, p_lambda);

        double lb = mst.first;
        if (lb > best_lb) {
            best_lb = lb;
            best_edges = mst.second;
            lambdas = p_lambda;
            iter = 0;
        }
        else {
            iter++;
            if (iter == iter_max) {
                iter = 0;
                eps /= 2;
            }
        }

        if (lb >= ub) break;

        int mx_deg = 0;
        for (int i = 0; i < n; i++) degrees[i] = 0;

        for (auto &[c, arc] : mst.second) {
            int i = arc.first;
            int j = arc.second;
            degrees[i]++;
            degrees[j]++;
            mx_deg = std::max({mx_deg, degrees[i], degrees[j]});
        }

        // Stop condition
        if (mx_deg == 2) break;

        double sqr_deg = 0.0;
        for (int i = 0; i < n; i++) {
            sqr_deg += (2 - degrees[i]) * (2 - degrees[i]);
        }

        double step = eps * ((ub - lb) / sqr_deg);
        for (int i = 0; i < n; i++) {
            p_lambda[i] = p_lambda[i] + step * (2 - degrees[i]);
        }
    }

    this->lower_bound = best_lb;
    this->graph = best_edges;
}

void Node::get_degrees() {
    for (int i = 0; i < n; i++) degrees[i] = 0;

    for (auto &[c, arc] : graph) {
        int i = arc.first;
        int j = arc.second;
        degrees[i]++;
        degrees[j]++;
    }
}

void Node::get_chosen() {
    int mx_deg = 0;
    for (int i = 0; i < n; i++) {
        if (mx_deg < degrees[i]) {
            mx_deg = degrees[i];
            chosen = i;
        }
    }
    
    chosen_node_arcs.clear();

    if (degrees[chosen] > 2) {
        for (auto &[c, arc] : graph) {
            int i = arc.first;
            int j = arc.second;
            
            if (i == chosen || j == chosen)
                chosen_node_arcs.push_back( { i, j });
        }
    }
}

bool Node::is_feasible() { return degrees[chosen] == 2; }
