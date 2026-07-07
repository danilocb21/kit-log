#ifndef SUBPROBLEM_H
#define SUBPROBLEM_H

#include <vector>
#include "combo.h"

using namespace std;

#define INF 2e18
#define bigM 1e6

struct SubProblem {
    int n;
    vector<double> price;
    vector<int> weight;
    double capacity, objVal;
    vector<int> solution;

    SubProblem(int n_, vector<double> &p, vector<int> &w, double c); 
    void solve();
};

#endif