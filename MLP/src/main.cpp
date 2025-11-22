#include <cstddef>
#include <iostream>
#include <vector>
#include <random>

#include "data.h"
#include "MLP.hpp"
#include "solution.hpp"

int main(int argc, char **argv) {
    srand(time(nullptr));
    
    auto data = Data(argc, argv[1]);
    data.read();

    MLP mlp = MLP(data);

    size_t max_iterations = 10;
    size_t max_ils_iterations = static_cast<size_t>(min(data.getDimension(), 100));
    
    std::vector<double> alphas;
    for (int i = 0; i <= 25; i++)
        alphas.push_back( i / 100.0 );
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> dist(0, 25);
    
    double alpha = alphas[dist(rng)];

    Solution s = mlp.GILS_RNVD(max_iterations, max_ils_iterations, alpha);
    s.print();

    return 0;
}
