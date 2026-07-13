#ifndef BP_H
#define BP_H

#include <vector>

#include "data.h"
#include "subproblem.h"
#include "node.h"
#include "gurobi_c++.h"

#define EPS 3e-6

class BP {
private:
    Data instance;

    // DFS Tree
    std::vector<Node> tree;
    
public:
    GRBEnv *env;

    double ub;

    int n;
    double capacity;
    std::vector<int> weight;

    void solve();
    void branch(GRBModel &model, std::vector<GRBVar> &lmbda, std::vector<GRBConstr> &constrs, int &n_lmbda);
    inline void column_gen(GRBModel &model, std::vector<GRBVar> &lmbda, std::vector<bool> &lmbd_itens, std::vector<GRBConstr> &constrs, int &n_lmbda, Node &node, int method);
    inline pair<double, Pair> most_fractional(GRBModel &model, std::vector<GRBVar> &lmbda, std::vector<bool> &lmbd_itens, std::vector<GRBConstr> &constrs, int &n_lmbda);
    inline int at(int i, int j);
    void print_results(const GRBModel &model, double duration, const std::string &instanceName);
    void print_solution(GRBModel &model, const std::vector<GRBVar> &vars, const std::vector<GRBConstr> &constrs);

    BP(Data &data, double ub_);
};

#endif