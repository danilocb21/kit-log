#ifndef BP_H
#define BP_H

#include <vector>

#include "data.h"
#include "gurobi_c++.h"

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
    void print_results(const GRBModel &model, const std::string &instanceName);
    void print_solution(const GRBModel &model);

    BP(Data &data, double ub_);
};

#endif