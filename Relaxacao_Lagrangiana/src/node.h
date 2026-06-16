#ifndef NODE_H
#define NODE_H

#include <vector>

#define INFINITE 99999999

typedef std::pair<int, int> Edge;

struct Node {
    int n;

    std::vector<std::pair<int, int>> forbidden_arcs;
    std::vector<Edge> graph;
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

    Node(int n) : lambdas(n) {};

    void update(std::vector<std::vector<double>> &costs, double ub);

    std::vector<Edge> subgradient_tsp(std::vector<std::vector<double>> &costs, double ub);

    std::vector<int> get_degrees(int n);
    int get_chosen();
    bool is_feasible();
};

#endif