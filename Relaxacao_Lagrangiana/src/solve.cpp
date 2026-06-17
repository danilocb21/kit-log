#include "bb.h"
#include "node.h"

double BB_TSP::solve(const BranchingStrategy branching_strategy) {
    double upper_bound = construction(); // Initial solution
    
    Node root(n);
    root.update(costs, upper_bound);
    
    if (branching_strategy == BranchingStrategy::DFS)
        upper_bound = solve_st(root, upper_bound);
    else if (branching_strategy == BranchingStrategy::BFS)
        upper_bound = solve_qu(root, upper_bound);
    else
        upper_bound = solve_pq(root, upper_bound);

    return upper_bound;
}

double BB_TSP::solve_pq(Node &root, double upper_bound) {
    pq_tree.push(root);
    
    while (!pq_tree.empty()) {
        Node node = pq_tree.top();
        pq_tree.pop();

        if (node.lower_bound > upper_bound)
            continue;

        if (node.is_feasible())
            upper_bound = std::min(upper_bound, node.lower_bound);
        else {
            for (int i = 0; i < (int)(node.chosen_node_arcs.size()); i++) {
                Node nd = node;

                nd.forbidden_arcs.emplace_back(
                    node.chosen_node_arcs[i].first,
                    node.chosen_node_arcs[i].second
                );

                nd.update(costs, upper_bound);
                if (nd.lower_bound <= upper_bound)
                    queue_tree.push(nd);
            }
        }
    }

    return upper_bound;
}

double BB_TSP::solve_st(Node &root, double upper_bound) {
    stack_tree.push(root);
    
    while (!stack_tree.empty()) {
        Node node = stack_tree.top();
        stack_tree.pop();

        if (node.lower_bound > upper_bound)
            continue;

        if (node.is_feasible())
            upper_bound = std::min(upper_bound, node.lower_bound);
        else {
            for (int i = 0; i < (int)(node.chosen_node_arcs.size()); i++) {
                Node nd = node;

                nd.forbidden_arcs.emplace_back(
                    node.chosen_node_arcs[i].first,
                    node.chosen_node_arcs[i].second
                );

                nd.update(costs, upper_bound);
                if (nd.lower_bound <= upper_bound)
                    queue_tree.push(nd);
            }
        }
    }

    return upper_bound;
}


double BB_TSP::solve_qu(Node &root, double upper_bound) {
    queue_tree.push(root);
    
    while (!queue_tree.empty()) {
        Node node = queue_tree.front();
        queue_tree.pop();

        if (node.lower_bound > upper_bound)
            continue;

        if (node.is_feasible())
            upper_bound = std::min(upper_bound, node.lower_bound);
        else {
            for (int i = 0; i < (int)(node.chosen_node_arcs.size()); i++) {
                Node nd = node;

                nd.forbidden_arcs.emplace_back(
                    node.chosen_node_arcs[i].first,
                    node.chosen_node_arcs[i].second
                );

                nd.update(costs, upper_bound);
                if (nd.lower_bound <= upper_bound)
                    queue_tree.push(nd);
            }
        }
    }

    return upper_bound;
}