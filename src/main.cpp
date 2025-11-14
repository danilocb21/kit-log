#include "Data.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <ctime>

using namespace std;

#define INF 2e18

int N;
struct Solution {
    vector<int> sequence;
    double cost;
};

struct InsertionInfo {
    int insertedNode;
    int removedEdge;
    double cost;

    bool operator<(const InsertionInfo &p) const {
        return cost < p.cost;
    }
};

vector<int> chooseRandomNodes();
set<int> remainingNodes(vector<int> &sequence);
vector<InsertionInfo> getInsertionCosts(Data &dt, vector<int> &seq);
Solution Construct(Data &dt);
Solution getSolutionILS(Data &dt, int maxIter, int maxIterIls);

int main(int argc, char** argv) {

    auto data = Data(argc, argv[1]);
    data.read();

    srand(time(NULL));

    N = data.getDimension();

    int maxIter = 50;
    int maxIterIls = N >= 150 ? N / 2 : N;

    Solution tsp = getSolutionILS(data, maxIter, maxIterIls);

    cout << "Solução: ";
    for (int x : tsp.sequence) 
        cout << x << ' ';
    cout << endl;
    cout << "Custo: " << tsp.cost << endl;

    return 0;
}

vector<int> chooseRandomNodes() {
    vector<int> sequence = { 1 };

    set<int> vis;
    vis.insert(1);

    // Escolhe 3 nós aleatórios para
    // compor a sequencia inicial
    while (sequence.size() < 4) {
        int node = rand() % N + 1;

        if (vis.count(node)) continue;

        vis.insert(node);
        sequence.push_back(node);
    }

    sequence.push_back(1);

    return sequence;
}

set<int> remainingNodes(vector<int> &sequence) {
    set<int> vis;
    for (int x : sequence) vis.insert(x);

    set<int> nodes;
    for (int i = 1; i <= N; i++) if (!vis.count(i))
        nodes.insert(i);
    
    return nodes;
}

vector<InsertionInfo> getInsertionCosts(Data &dt, vector<int> &seq, set<int> &candidates) {
    vector<InsertionInfo> s;
    s.reserve((seq.size() - 1) * (candidates.size()));
    
    for (int a = 0; a < seq.size() - 1; a++) {
        int i = seq[a];
        int j = seq[a + 1];
        for (int k : candidates) {
            double cost = dt.getDistance(i,k) + dt.getDistance(k,j) - dt.getDistance(i,j);
            s.push_back({k, a, cost});
        }
    }

    return s;
}

// Inserção mais barata
Solution Construct(Data &dt) {
    Solution sol{};
    sol.sequence = chooseRandomNodes();
    auto &seq = sol.sequence;
    auto CL = remainingNodes(seq);
    
    while (!CL.empty()) {
        auto insertionCosts = getInsertionCosts(dt, seq, CL);
        sort(insertionCosts.begin(), insertionCosts.end());

        double alpha = (double) rand() / RAND_MAX;
        int selected = rand() % ((int)ceil(alpha * insertionCosts.size()));
        
        auto &x = insertionCosts[selected];
        CL.erase(x.insertedNode);
        seq.insert(seq.begin() + x.removedEdge + 1, x.insertedNode);
    }

    for (int a = 0; a < seq.size() - 1; a++) {
        int i = seq[a];
        int j = seq[a + 1];
        sol.cost += dt.getDistance(i,j);
    }

    return sol;
}

Solution getSolutionILS(Data &dt, int maxIter, int maxIterIls) {
    Solution bestOfAll;
    bestOfAll.cost = INF;

    for (int i = 0; i < maxIter; i++) {
        Solution s = Construct(dt);
        Solution best = s;

        if (best.cost < bestOfAll.cost)
            bestOfAll = best;
    }

    return bestOfAll;
}
