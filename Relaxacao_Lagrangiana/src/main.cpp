#include <iostream>
#include <chrono>

#include "data.h"
#include "bb.h"

int main(int argc, char** argv) {

	Data data = Data(argc, argv[1]);
	data.read();

	BB_TSP tsp(data);

	std::string strategy = argv[2];
	BB_TSP::BranchingStrategy branching_strategy;
	
	if (strategy == "DFS")
		branching_strategy = BB_TSP::BranchingStrategy::DFS;
	else if (strategy == "BFS")
		branching_strategy = BB_TSP::BranchingStrategy::BFS;
	else
		branching_strategy = BB_TSP::BranchingStrategy::BBS;

	
	int runs = 1;
    double total_time = 0.0, total_cost = 0.0;

    for (int i = 0; i < runs; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        double s = tsp.solve(branching_strategy);

        auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> duration = end - start;
        total_time += duration.count();
        total_cost += s;
    }
    
    std::cout << total_time / runs << ' ' << total_cost / runs << '\n';

	return 0;
}
