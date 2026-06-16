#ifndef BNB_HPP
#define BNB_HPP

#include <cstdint>
#include <vector>
#include <stack>
#include <queue>

#include "data.h"
#include "node.hpp"

class BnB {
private:
    Data m_instance;
    
    std::stack<Node> tree1;
    std::queue<Node> tree2;
    
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq_tree;

public:
    enum class BranchingStrategy : uint8_t { DFS, BFS, BBS };
    double **costs;
    int N; // grid dimension

    // Construct a solution with a greedy heuristic and returns its cost
    double construction();

    // Branch and Bound algorithm
    double solve(const BranchingStrategy branching_strategy);
    double solve_pq(Node &root, double upper_bound); // Best Bound Search
    double solve_list(Node &root, double upper_bound); // DFS
    double solve_list2(Node &root, double upper_bound); // BFS

    BnB(Data &instance, double **cost);
};

#endif