#include <cmath>

#include "subproblem.h"
#include "combo.h"

SubProblem::SubProblem(int n_, vector<double> &p, vector<int> &w, double c) : n(n_), price(p), weight(w), capacity(c) {
    objVal = INF;
    solution.resize(n);
}

void SubProblem::solve() {
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