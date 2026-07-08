#ifndef BP_H
#define BP_H

#include <vector>

#include "data.h"
#include "gurobi_c++.h"

#define EPS 3e-6

class BP {
private:
    Data instance;

    
public:
    GRBEnv *env;

    double ub;

    int n;
    double capacity;
    std::vector<int> weight;

    void solve();
    inline void column_gen(GRBModel &model, std::vector<GRBVar> &lmbda, std::vector<GRBConstr> &constrs, int &n_lmbda);
    void print_results(const GRBModel &model, double duration, const std::string &instanceName);
    void print_solution(GRBModel &model, const std::vector<GRBVar> &vars, const std::vector<GRBConstr> &constrs);

    BP(Data &data, double ub_);
};

#endif