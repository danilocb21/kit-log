#ifndef NODE_H
#define NODE_H

#include <vector>

#define INFINITE 99999999

typedef std::pair<double, std::pair<int, int>> Edge;

struct Node {
    int n;

    std::vector<std::pair<int, int>> forbidden_arcs;
    std::vector<Edge> graph;
    std::vector<std::pair<int, int>> chosen_node_arcs;
    std::vector<int> degrees;
    std::vector<double> lambdas;
    double lower_bound;
    int chosen;
    bool feasible;

    bool operator<(const Node &p) const {
        return lower_bound < p.lower_bound;
    }

    bool operator>(const Node &p) const {
        return lower_bound > p.lower_bound;
    }

    Node(int n_) {
        n = n_;
        degrees.resize(n);
        lambdas.resize(n);
    };

    void update(std::vector<std::vector<double>> &costs, double ub);

    void subgradient_tsp(std::vector<std::vector<double>> &costs, double ub);

    void get_degrees();
    int get_chosen();
    bool is_feasible();
};

#endif