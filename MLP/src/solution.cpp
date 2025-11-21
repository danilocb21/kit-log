#include <iostream>

#include "solution.hpp"

void Solution::print() const {
    std::cout << "\nSolution\n";

    for (int x : sequence)
        std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "Cost: " << cost << '\n';
}