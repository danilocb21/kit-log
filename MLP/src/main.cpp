#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#include "data.h"
#include "MLP.hpp"
#include "solution.hpp"

int main(int argc, char **argv) {
    auto data = Data(argc, argv[1]);
    data.read();

    MLP mlp = MLP(data);

    uint8_t max_iterations = 10;
    uint8_t max_ils_iterations = std::min(data.getDimension(), 100);

    uint8_t runs = 10;
    double total_time = 0.0, total_cost = 0.0;

    for (uint8_t i = 0; i < runs; i++) {

        auto start = std::chrono::high_resolution_clock::now();

        Solution s = mlp.GILS_RVND(max_iterations, max_ils_iterations);

        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        total_time += duration.count();
        total_cost += s.cost;
    }
    
    std::cout << total_time / runs << ' ' << total_cost / runs << '\n';

    return 0;
}
