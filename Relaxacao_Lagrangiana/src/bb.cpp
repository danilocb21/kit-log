#include <vector>

#include "data.h"
#include "bb.h"

BB_TSP::BB_TSP(Data &instance) : m_instance(instance) {
    n = m_instance.getDimension();

    costs.resize(n+1, std::vector<double>(n+1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            costs[i][j] = m_instance.getDistance(i,j);
        }
    }
}