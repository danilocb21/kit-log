#include <algorithm>
#include <random>

#include "MLP.hpp"
#include "solution.hpp"

void MLP::perturbation(Solution &solution) {
    size_t n = m_instance.getDimension();
    size_t lmin = 2;
    size_t lmax = (n + 9) / 10; // ceil(N/10)
    if (n < 6 || lmax < 2) return;

    auto has_intersection = [](size_t a, size_t b, size_t c, size_t d) -> bool {
        return max(a, c) <= min(b, d);
    };

    size_t s1, s2, len1, len2;

    std::uniform_int_distribution<> dist(lmin, lmax);
    
    do {
        len1 = dist(m_generator);
        len2 = dist(m_generator);

        std::uniform_int_distribution<> d1(1, n - 2 - len1);
        std::uniform_int_distribution<> d2(1, n - 2 - len2);

        s1 = d1(m_generator);
        s2 = d2(m_generator);

    } while (has_intersection(s1, s1 + len1 - 1, s2, s2 + len2 - 1));

    if (s1 > s2) {
        swap(s1,s2);
        swap(len1,len2);
    }

    int v1_f = solution.sequence[s1];
    int v1_b = solution.sequence[s1 + len1 - 1];
    int v1_prev = solution.sequence[s1 - 1];
    int v1_next = solution.sequence[s1 + len1];

    int v2_f = solution.sequence[s2];
    int v2_b = solution.sequence[s2 + len2 - 1];
    int v2_prev = solution.sequence[s2 - 1];
    int v2_next = solution.sequence[s2 + len2];

    double delta;
    if (v1_next == v2_f)
        delta =   dist_matrix[v1_prev][v2_f] + dist_matrix[v2_b][v1_f]
                + dist_matrix[v1_b][v2_next] - dist_matrix[v1_prev][v1_f]
                - dist_matrix[v1_b][v2_f]    - dist_matrix[v2_b][v2_next];
    else
        delta =   dist_matrix[v1_prev][v2_f] + dist_matrix[v2_b][v1_next]
                + dist_matrix[v2_prev][v1_f] + dist_matrix[v1_b][v2_next]
                - dist_matrix[v1_prev][v1_f] - dist_matrix[v1_b][v1_next]
                - dist_matrix[v2_prev][v2_f] - dist_matrix[v2_b][v2_next];

    solution.cost += delta;
    auto first_seg  = solution.sequence.begin() + s1;
    auto second_seg = solution.sequence.begin() + s2;
    auto mid1 = first_seg  + len1;
    auto mid2 = second_seg + len2;

    rotate(first_seg, mid1, second_seg);
    rotate(first_seg, second_seg, mid2);
}