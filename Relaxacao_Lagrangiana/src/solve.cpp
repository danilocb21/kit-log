#include "BnB.hpp"
#include "node.hpp"

double BnB::solve(const BranchingStrategy branching_strategy) {
    Node root;
    root.update(costs, N);
    
    double upper_bound = construction(); // Initial solution    

    if (branching_strategy == BranchingStrategy::BBS)
        upper_bound = solve_pq(root, upper_bound);
    else if (branching_strategy == BranchingStrategy::DFS)
        upper_bound = solve_list(root, upper_bound);
    else
        upper_bound = solve_list2(root, upper_bound);

    return upper_bound;
}

double BnB::solve_pq(Node &root, double upper_bound) {
    pq_tree.push(root);
    
    while (!pq_tree.empty()) {
        Node node = pq_tree.top();
        pq_tree.pop();

        if (node.lower_bound > upper_bound)
            continue;

        if (node.is_feasible())
            upper_bound = std::min(upper_bound, node.lower_bound);
        else {
            int chosen = node.chosen;
            for (size_t i = 0; i < node.subtour[chosen].size() - 1; i++) {
                Node n = node;

                n.forbidden_arcs.emplace_back(
                    node.subtour[chosen][i],
                    node.subtour[chosen][i + 1]
                );

                n.update(costs, N);
                if (n.lower_bound <= upper_bound)
                    pq_tree.push(n);
            }
        }
    }

    return upper_bound;
}

double BnB::solve_list(Node &root, double upper_bound) {
    tree1.push(root);
    
    while (!tree1.empty()) {
        Node node = tree1.top();
        tree1.pop();

        if (node.lower_bound > upper_bound)
            continue;

        if (node.is_feasible())
            upper_bound = std::min(upper_bound, node.lower_bound);
        else {
            int chosen = node.chosen;
            for (size_t i = 0; i < node.subtour[chosen].size() - 1; i++) {
                Node n = node;

                n.forbidden_arcs.emplace_back(
                    node.subtour[chosen][i],
                    node.subtour[chosen][i + 1]
                );

                n.update(costs, N);
                if (n.lower_bound <= upper_bound)
                    tree1.push(n);
            }
        }
    }

    return upper_bound;
}


double BnB::solve_list2(Node &root, double upper_bound) {
    tree2.push(root);
    
    while (!tree2.empty()) {
        Node node = tree2.front();
        tree2.pop();

        if (node.lower_bound > upper_bound)
            continue;

        if (node.is_feasible())
            upper_bound = std::min(upper_bound, node.lower_bound);
        else {
            int chosen = node.chosen;
            for (size_t i = 0; i < node.subtour[chosen].size() - 1; i++) {
                Node n = node;

                n.forbidden_arcs.emplace_back(
                    node.subtour[chosen][i],
                    node.subtour[chosen][i + 1]
                );

                n.update(costs, N);
                if (n.lower_bound <= upper_bound)
                    tree2.push(n);
            }
        }
    }

    return upper_bound;
}