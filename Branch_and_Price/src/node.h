#ifndef NODE_H
#define NODE_H

#include <vector>

using namespace std;

typedef pair<int,int> Pair;

struct Node {
    double lb = 0.0;
    Pair most_fract;
    double most_fract_val;
    vector<Pair> frbnd_pairs;
    vector<Pair> reqrd_pairs;
    vector<double> lmbda_val;
    vector<bool> added_pair;

    bool is_feasible();
};

#endif