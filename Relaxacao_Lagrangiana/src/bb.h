#ifndef BB_H
#define BB_H

#include <vector>
#include <stack>
#include <queue>

#include "data.h"
#include "solution.h"
#include "node.h"

class BB_TSP {
private:
    Data m_instance;
    
    std::stack<Node> stack_tree;
    std::queue<Node> queue_tree;
    
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq_tree;

public:
    enum class BranchingStrategy : uint8_t { DFS, BFS, BBS };
    std::vector<std::vector<double>> costs;
    int n; // grid dimension

    // Construct a solution with a greedy heuristic
    Solution construction();

    // Branch and Bound algorithm
    double solve(const BranchingStrategy branching_strategy);
    double solve_st(Node &root, double upper_bound); // DFS
    double solve_qu(Node &root, double upper_bound); // BFS
    double solve_pq(Node &root, double upper_bound); // BBS

    BB_TSP(Data &instance);
};

#endif