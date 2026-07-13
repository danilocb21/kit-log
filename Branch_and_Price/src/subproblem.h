#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include <vector>
#include "combo.h"
#include "node.h"
#include "gurobi_c++.h"

using namespace std;

#define INF 2e18
#define bigM 1e6

#define SOLVE_MODEL 0
#define SOLVE_DP 1
struct SubProblem {
    int n;
    vector<double> price;
    vector<int> weight;
    double capacity, objVal;
    vector<int> solution;

    SubProblem(int n_, vector<double> &p, vector<int> &w, double c);
    void solve(int method, Node &node, GRBEnv* env);
    void solve_dp();
    void solve_model(Node &node, GRBEnv* env);
};

#endif