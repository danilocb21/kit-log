#include <cstddef>
#include <algorithm>

#include "MLP.hpp"
#include "solution.hpp"

bool MLP::best_improvement_swap(Solution &solution) {
    double best_delta = 0.0;
    size_t best_i, best_j;

    for (size_t i = 1; i < solution.sequence.size() - 1; i++) {
        int vi      = solution.sequence[i];
        int vi_prev = solution.sequence[i - 1];
        int vi_next = solution.sequence[i + 1];

        for (size_t j = i + 1; j < solution.sequence.size() - 1; j++) {
            int vj      = solution.sequence[j];
            int vj_prev = solution.sequence[j - 1];
            int vj_next = solution.sequence[j + 1];

            double delta;
            if (i + 1 == j)
                delta =   dist_matrix[vi_prev][vj]    + dist_matrix[vj][vi] 
                        + dist_matrix[vi][vj_next]    - dist_matrix[vi_prev][vi]
                        - dist_matrix[vi][vj]         - dist_matrix[vj][vj_next];
            else
                delta =   dist_matrix[vi_prev][vj]   + dist_matrix[vj][vi_next] 
                        + dist_matrix[vj_prev][vi]   + dist_matrix[vi][vj_next] - dist_matrix[vi_prev][vi]
                        - dist_matrix[vi][vi_next]   - dist_matrix[vj_prev][vj] - dist_matrix[vj][vj_next];

            if (delta < best_delta) {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (best_delta < 0) {
        swap(solution.sequence[best_i], solution.sequence[best_j]);
        solution.cost += best_delta;
        return true;
    }

    return false;
}

bool MLP::best_improvement_two_opt(Solution &solution) {
    double best_delta = 0.0;
    size_t best_i, best_j;

    for (size_t i = 0; i < solution.sequence.size() - 1; i++) {
        int vi      = solution.sequence[i];
        int vi_next = solution.sequence[i + 1];

        for (size_t j = i + 2; j < solution.sequence.size() - 1; j++) {
            int vj      = solution.sequence[j];
            int vj_next = solution.sequence[j + 1];

            double delta =   dist_matrix[vi][vj]      + dist_matrix[vi_next][vj_next]
                           - dist_matrix[vi][vi_next] - dist_matrix[vj][vj_next];

            if (delta < best_delta) {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    if (best_delta < 0) {
        std::reverse(solution.sequence.begin() + best_i + 1, solution.sequence.begin() + best_j + 1);
        solution.cost += best_delta;
        return true;
    }

    return false;
}

bool MLP::best_improvement_or_opt(Solution &solution, int block_size) {
    double best_delta = 0.0;
    size_t best_i, best_j;

    for (size_t i = 0; i < solution.sequence.size() - block_size - 1; i++) {
        int vi       = solution.sequence[i];
        int vi_next  = solution.sequence[i + block_size + 1];
        int vk_first = solution.sequence[i + 1];
        int vk_last  = solution.sequence[i + block_size];

        for (size_t j = 0; j < solution.sequence.size() - 1; j++) {
            // j >= i && j <= i + blockSize
            if (i == j) {
                j += block_size;
                continue;
            }
            int vj = solution.sequence[j];
            int vj_next = solution.sequence[j + 1];

            double delta =   dist_matrix[vi][vi_next]      + dist_matrix[vj][vk_first]
                           + dist_matrix[vk_last][vj_next] - dist_matrix[vi][vk_first]
                           - dist_matrix[vk_last][vi_next] - dist_matrix[vj][vj_next];

            if (delta < best_delta) {
                best_i = i;
                best_j = j;
                best_delta = delta;
            }
        }
    }

    if (best_delta < 0) {
        auto block_start = solution.sequence.begin() + best_i + 1;
        auto block_end   = solution.sequence.begin() + best_i + 1 + block_size;
        auto segment     = solution.sequence.begin() + best_j + 1;

        if (best_i < best_j)
            rotate(block_start, block_end, segment);
        else
            rotate(segment, block_start, block_end);

        solution.cost += best_delta;
        return true;
    }

    return false;
}