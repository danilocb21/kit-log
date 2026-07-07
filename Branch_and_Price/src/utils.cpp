#include <iostream>
#include <string>

#include "BP.h"
#include "gurobi_c++.h"

using namespace std;

void BP::print_results(const GRBModel &model, const string &instanceName) {
    cout << "\n\nBranch-and-Price_Results: \n" << endl;

	cout << "Instance: " << instanceName << endl;
    cout << "Tree_Size: " << model.get(GRB_DoubleAttr_NodeCount) 
                            + model.get(GRB_DoubleAttr_OpenNodeCount) << endl;
    cout << "Total_Time: " << model.get(GRB_DoubleAttr_Runtime) << endl;
    cout << "Total_Memory_Used (MB): " << model.get(GRB_DoubleAttr_MaxMemUsed) * 1024 << endl;
    cout << "LB: " << model.get(GRB_DoubleAttr_ObjBound) << endl;
    cout << "UB: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
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

void BP::print_solution(const GRBModel &model) {
    
}