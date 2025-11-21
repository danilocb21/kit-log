#include <limits>

#include "MLP.hpp"
#include "solution.hpp"

Solution MLP::GILS_RNVD(const size_t max_iterations, const size_t max_ils_iterations, const double alpha) {
    Solution best_found;
    best_found.cost = std::numeric_limits<double>::max();

    for (size_t i = 0; i < max_iterations; i++) {
        Solution solution = rand_cheapest_insertion(alpha);
        Solution best = solution;

        size_t iteration_ils = 0;
        
        while (iteration_ils <= max_ils_iterations) {
            RVND(solution);

            if (solution.cost < best.cost) {
                best = solution;
                iteration_ils = 0;
            }
            else if (solution.cost > best.cost)
                solution = best;

            perturbation(solution);
            iteration_ils++;
        }

        if (best.cost < best_found.cost)
            best_found = best;
    }

    return best_found;
}