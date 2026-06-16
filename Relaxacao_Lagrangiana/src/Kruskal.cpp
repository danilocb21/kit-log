#include "Kruskal.h"

int Kruskal::get(int x) {
    return prnt[x] = (prnt[x] == x) ? x : get(prnt[x]);
}

void Kruskal::union_set(int a, int b) {
    a = get(a), b = get(b);
    if (same_set(a,b)) return;

    if (rank[a] < rank[b])
        std::swap(a,b);
    
    if (rank[a] == rank[b])
        rank[a]++;
    prnt[b] = a;
}

bool Kruskal::same_set(int a, int b) {
    return prnt[get(a)] == prnt[get(b)];
}

std::pair<double, std::vector<Edge>> Kruskal::find_mst(const std::vector<std::vector<double>> &costs, 
                                                    const std::vector<double> &lambdas) {
    std::vector<Edge> edges;

    // i = 0 é a origem
    for (int i = 1; i+1 < n; i++) {
        for (int j = i+1; j < n; j++) {
            double partial_cost = costs[i][j] - lambdas[i] - lambdas[j];
            edges.push_back({ partial_cost, { i, j }});
        }
    }

    sort(edges.begin(), edges.end());
    
    double cost = 0.0;
    std::vector<Edge> mst;

    for (auto &ed : edges) {
        double c = ed.first;
        int i = ed.second.first;
        int j = ed.second.second;

        if (same_set(i,j)) continue;
        union_set(i,j);
        cost += c;
        mst.push_back(ed);
    }

    // Conectar origem a MST
    Edge p1 = { INFINITE, { 0,0 }}, p2 = { INFINITE, { 0, 0 }};
    for (int j = 1; j < n; j++) {
        double c = costs[0][j] - lambdas[0] - lambdas[j];
        if (c < p1.first) {
            p2 = p1;
            p1 = { c, { 0, j }};
        }
        else if (c < p2.first) {
            p2 = { c, { 0, j }};
        }
    }

    if (p1.first < INFINITE) {
        cost += p1.first;
        mst.push_back(p1);
    }

    if (p2.first < INFINITE) {
        cost += p2.first;
        mst.push_back(p2);
    }

    return std::make_pair(cost, mst);
}