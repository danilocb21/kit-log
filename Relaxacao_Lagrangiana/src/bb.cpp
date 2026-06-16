#include <vector>

#include "data.h"
#include "bb.h"

BB_TSP::BB_TSP(Data &instance) : m_instance(instance) {
    n = m_instance.getDimension();

    costs.resize(n, std::vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            costs[i][j] = m_instance.getDistance(i+1,j+1);
        }
    }
}