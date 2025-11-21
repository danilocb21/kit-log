#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>

struct Solution {
    std::vector<int> sequence;
    double cost;

    Solution() = default;

    void print() const;
};

#endif