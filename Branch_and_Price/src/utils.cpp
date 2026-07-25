#include <iostream>
#include <string>

#include "BP.h"
#include "gurobi_c++.h"

using namespace std;

void BP::print_results(const GRBModel& model, double duration, const string& instanceName) {
    cout << "\n\nBranch-and-Price_Results: \n" << endl;

	cout << "Instance: " << instanceName << endl;
    cout << "Total_Time: " << duration << endl;
    // cout << "Total_Memory_Used (MB): " << model.get(GRB_DoubleAttr_MaxMemUsed) * 1024 << endl;
    // cout << "LB: " << model.get(GRB_DoubleAttr_ObjBound) << endl;
    cout << "UB: " << ub << endl;
    cout << "Status: ";
    switch (model.get(GRB_IntAttr_Status)) {
    case GRB_OPTIMAL:
        cout << "Optimal";
        break;
    
    case GRB_INFEASIBLE:
        cout << "Infeasible";
        break;
    
    case GRB_INF_OR_UNBD:
        cout << "Infeasible/Unbounded";
        break;
    
    case GRB_UNBOUNDED:
        cout << "Unbounded";
        break;

    default:
        cout << "Didn't solve ( " << model.get(GRB_IntAttr_Status) << " )";
        break;
    }
    cout << endl;
}

void BP::print_solution(GRBModel& model, const std::vector<bool>& lmbd_itens) {
    cout << "\nSolution:\n";

    int numVars = (int) solution.size();
    int numConstrs = n;

    int bins = 0;
    for (int j = 0; j < numVars; j++) {
        double xj = solution[j];
        if (xj <= EPS) continue;
        bins++;
        cout << "Bin " << bins << ": ";

        for (int i = 0; i < numConstrs; i++) {
            if (lmbd_itens[at(j,i)]) 
                cout << 'x' << i << ' ';
        }
        cout << endl;
    }
}

inline int BP::at(int i, int j) {
    return i * n + j;
}