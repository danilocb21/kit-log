#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <algorithm>
#include <vector>
#include <numeric>

#define INFINITE 99999999

typedef std::pair<double, std::pair<int, int>> Edge;

class Kruskal {
private:
    int n;
    std::vector<int> prnt, rank;

public:
    Kruskal(int n_) {
        n = n_;
        prnt.resize(n);
        rank.resize(n);
        std::iota(prnt.begin(), prnt.end(), 0);
    }

    int get(int x);
    void union_set(int a, int b);
    bool same_set(int a, int b);

    std::pair<double, std::vector<Edge>> find_mst(const std::vector<std::vector<double>> &costs, 
                                                    const std::vector<double> &lambdas);
};

#endif