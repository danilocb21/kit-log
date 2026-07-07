#include <cmath>

#include "subproblem.h"
#include "combo.h"
#include "gurobi_c++.h"

SubProblem::SubProblem(int n_, vector<double> &p, vector<int> &w, double c) : n(n_), price(p), weight(w), capacity(c) {
    objVal = INF;
    solution.resize(n);
}

void SubProblem::solve(int method, GRBEnv* env) {
    if (method == SOLVE_DP)
        solve_dp();
    else
        solve_model(env);
}

void SubProblem::solve_dp() {
    vector<item> items(n);

    for (int i = 0; i < n; ++i) {
        items[i].p = (itype) llround(price[i] * bigM);
        items[i].w = (itype) weight[i];
        items[i].x = 0;
        items[i].index = i;
    }

    stype bestProfit = combo(
        items.data(),
        items.data() + n - 1,
        (stype) std::llround(capacity),
        0,
        0,
        true,
        true
    );

    objVal = 1.0 - (double) (bestProfit) / bigM;

    for (const auto &it : items) {
        solution[it.index] = it.x;
    }
}

void SubProblem::solve_model(GRBEnv* env) {
    GRBModel model = GRBModel(*env);

    model.set(GRB_StringAttr_ModelName, "Knapsack Problem");
    model.set(GRB_DoubleParam_TimeLimit, 1*60*60);
    model.set(GRB_IntParam_Threads, 1);

    vector<GRBVar> x(n);
    for (int i = 0; i < n; i++) {
        string vx = "x[" + to_string(i+1) + "]";
        x[i] = model.addVar(0.0, 1.0, price[i], GRB_BINARY, vx.c_str());
    }

    GRBLinExpr expr = 0.0;
    for (int i = 0; i < n; i++) {
        expr += weight[i] * x[i];
    }
    model.addConstr(expr <= capacity, "c1");

    model.set(GRB_IntAttr_ModelSense, GRB_MAXIMIZE);
    model.optimize();

    double bestProfit = model.get(GRB_DoubleAttr_ObjVal);
    objVal = 1.0 - bestProfit;

    for (int i = 0; i < n; i++) {
        solution[i] = x[i].get(GRB_DoubleAttr_X);
    }
}