#include <vector>
#include <random>

#include "MLP.hpp"
#include "solution.hpp"

void MLP::RVND(Solution &solution) {
    vector<Neighborhood> NL = { Neighborhood::Swap, Neighborhood::TwoOpt, Neighborhood::Reinsertion,
                                Neighborhood::OrOpt2, Neighborhood::OrOpt3 };
    bool improved = false;

    while (!NL.empty()) {
        std::uniform_int_distribution<int> dist(0, NL.size() - 1);
        int n = dist(m_generator);
        switch (NL[n])
        {
        case Neighborhood::Swap:
            improved = best_improvement_swap(solution);
            break;
        case Neighborhood::TwoOpt:
            improved = best_improvement_two_opt(solution);
            break;
        case Neighborhood::Reinsertion:
            improved = best_improvement_or_opt(solution, 1);
            break;
        case Neighborhood::OrOpt2:
            improved = best_improvement_or_opt(solution, 2);
            break;
        case Neighborhood::OrOpt3:
            improved = best_improvement_or_opt(solution, 3);
            break;
        }

        if (improved)
            NL = { Neighborhood::Swap, Neighborhood::TwoOpt, Neighborhood::Reinsertion,
                    Neighborhood::OrOpt2, Neighborhood::OrOpt3 };
        else
            NL.erase(NL.begin() + static_cast<size_t> (n));
    }
}