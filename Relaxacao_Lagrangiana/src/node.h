#ifndef NODE_H
#define NODE_H

#include <vector>

struct Node {
    std::vector<std::pair<int, int>> forbidden_arcs;
    std::vector<std::vector<int>> subtour;
    double lower_bound;
    int chosen;
    bool feasible;

    bool operator<(const Node &p) const {
        return lower_bound < p.lower_bound;
    }

    bool operator>(const Node &p) const {
        return lower_bound > p.lower_bound;
    }

    Node() = default;

    void update(double **costs, int n);

    // Hungarian Instance info

    //std::vector<std::vector<int>> get_subtours(hungarian_problem_t &p, int n);
    int get_chosen();
    bool is_feasible();
};

#endif